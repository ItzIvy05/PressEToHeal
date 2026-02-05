#include "Logging.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace Logging {
    void Init() {
        auto logger = spdlog::basic_logger_mt("RequiemLP", "RequiemLP.log", true);
        spdlog::set_default_logger(std::move(logger));
        spdlog::set_pattern("%v");
    }
}
