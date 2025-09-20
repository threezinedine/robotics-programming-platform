#include "core/logging.h"

namespace rpp
{
    /**
     * @brief A log handler that outputs log messages to the console (print to standard output).
     */
    class ConsoleHandler : public Handler
    {
    public:
        ConsoleHandler();
        ~ConsoleHandler() override;

    protected:
        void HandleImpl(const LogRecord &record) override;
    };
} // namespace rpp
