#pragma once

#include <string>

namespace docker::request_params {

struct CreateContainer {
    std::string image;
    bool tty{true};
    uint64_t memory{0};  // bytes
};

struct RemoveContainer {
    std::string containerId;
    bool volume{false};
    bool force{false};
};

struct KillContainer {
    std::string containerId;
    std::string signal;  // e.g. "SIGKILL"
};

using RunContainer = CreateContainer;

struct PutArchive {
    std::string containerId;
    std::string path;
    std::string archive;  // in memory tar archive
};

struct ExecCreate {
    std::string containerId;
    std::vector<std::string> cmd;
    bool attachStderr{true};
    bool attachStdout{true};
    bool detach{false};
    bool tty{true};
};

// extra options
struct ExecStart {
    std::string execId;
    bool detach{false};
    bool tty{true};
};

}  // namespace docker::request_params