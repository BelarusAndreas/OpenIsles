#ifndef _DEFINES_H
#define _DEFINES_H

// Bestimmte Methoden sind nur dann virtuell, wenn wir die Tests kompilieren. Wir sparen uns für die richtige
// Anwendung den vtable-Overhead :-)
#ifdef IN_TESTS
    #define VIRTUAL_ONLY_IN_TESTS virtual
#else
    #define VIRTUAL_ONLY_IN_TESTS
#endif

// Helper für Performance-Messungen. Mit dem limit-Parameter kann ab einer zu hohen Zeit zwischen stderr/stdout
// unterschieden werden, damit in der IDE hohe Zahlen in Rot dargestellt werden.
#ifdef DEBUG
    #include <chrono>
    #include <iostream>

    #define PERFORMANCE_START(name) \
        std::chrono::steady_clock::time_point performanceCounterStart ## name = std::chrono::steady_clock::now();

    #define PERFORMANCE_END(name, limit) \
        std::chrono::steady_clock::time_point performanceCounterEnd ## name = std::chrono::steady_clock::now(); \
        auto performanceCounterTime ## name = \
            std::chrono::duration_cast<std::chrono::milliseconds>( \
                performanceCounterEnd ## name - performanceCounterStart ## name).count(); \
        \
        if (performanceCounterTime ## name > limit) { \
            std::cerr << #name << " took " << performanceCounterTime ## name << "ms" << std::endl; \
        } else { \
            std::cout << #name << " took " << performanceCounterTime ## name << "ms" << std::endl; \
        }
#else
    #define PERFORMANCE_START(name)
    #define PERFORMANCE_END(name)
#endif

// Test-Helper
#ifdef IN_TESTS
    /**
     * @brief Assert-Helper, um Endlosschleifen im Spiel zu vermeiden.
     * Überschreitet die Spielzeit einen bestimmten Zeitpunkt, wird der Test abgebrochen.
     * Die Variable `game` muss verfügbar sein (automatisch in von GameTest abgeleiteten Tests).
     *
     * @param maxGameTicks Höchstspielzeit, die erreicht werden darf.
     * @param additionalErrorInfo Zusätzliche Information, die in der Fehlermeldung ausgegeben wird
     */
    #define ASSERT_NO_ENDLESS_LOOP(maxGameTicks, additionalErrorInfo) \
        if (game->getTicks() > (maxGameTicks)) { \
            FAIL() << "Endless loop detected - " #additionalErrorInfo ; \
        }
#endif

static const unsigned long TICKS_PER_SECOND = 1000;                  ///< Ticks pro Sekunde
static const unsigned long TICKS_PER_MINUTE = 60 * TICKS_PER_SECOND; ///< Ticks pro Minute

#endif