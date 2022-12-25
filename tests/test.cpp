#include "docker.h"

void test_version() {
    Docker client = Docker();

    JSON_DOCUMENT doc = client.docker_version();
    std::cout << jsonToString(doc) << std::endl;
}

void test_list_images() {
    Docker client = Docker();

    auto images = client.list_images();
    for (auto &image: images.GetObject()) {
        std::cout << jsonToString(image.name) << ':';
        std::cout << jsonToString(image.value) << std::endl;
    }
}

void test_create_container() {
    Docker client = Docker();

    std::string const createParams = R"({"Image": "senjun_courses_python", "Tty": true, "HostConfig": {"Memory": 7000000}})";
    rapidjson::Document params;
    if (params.Parse(createParams.c_str()).HasParseError()) {
        assert(false);
    }
    auto result = client.create_container(params);
    std::cout << jsonToString(result["success"]) << ": ";
    std::cout << jsonToString(result["data"]);
}

void test_run_container() {
    Docker client = Docker();

    std::string const runParams =
            R"({"Image": "senjun_courses_python", "Tty": true, "HostConfig": {"Memory": 7000000}})";
    rapidjson::Document params;
    if (params.Parse(runParams.c_str()).HasParseError()) {
        assert(false);
    }
    auto result = client.run_container(params);
    assert(result["success"].GetBool());
    assert(result["data"].GetObject()["Id"].GetString());

    std::string const id = result["data"].GetObject()["Id"].GetString();
    std::cout << "Container created: " << id << std::endl;

    result = client.kill_container(id);
    assert(result["success"].GetBool());

    result = client.delete_container(id);
    assert(result["success"].GetBool());
    std::cout << "Container removed: " << id << std::endl;
}

void test_put_archive() {
    std::string const id = "sad_antonelli";
    std::string const pathInContainer = "/home/code_runner";
    std::string const pathToTar = "/Users/d.shipilov/Shipilov/SenJun/main.cpp.tgz";

    Docker client;
    auto result = client.put_archive(id, pathInContainer, pathToTar);
    assert(result["success"].GetBool());
}

void test_exec() {
    std::string const id = "objective_cori";

    // m.py — file inside container for launching
    std::string const strExecParams =
            R"({"AttachStderr": true, "AttachStdout": true, "Detach": false, "Tty": true, "Cmd": ["sh",  "run.sh", " m.py"]})";
    rapidjson::Document execParams;
    if (execParams.Parse(strExecParams.c_str()).HasParseError()) {
        assert(false);
    }

    rapidjson::Document startParams;

    std::string const strStartParams =
            R"({"Detach": false, "Tty": false})";
    if (startParams.Parse(strStartParams.c_str()).HasParseError()) {
        assert(false);
    }

    Docker client;
    auto result = client.exec(execParams, startParams, id);
    if (!result["success"].GetBool()) {
        assert(false);
    }

    std::string const answer = jsonToString(result["data"]);
    std::cout << "[0]: " << answer[0] << std::endl;
    std::cout << "[1]: " << answer[1] << std::endl;
    std::cout << "[2]: " << answer[2] << std::endl;
    std::cout << "[3]: " << answer[3] << std::endl;

    std::cout << "[n-1]: " << answer[answer.size()-1] << std::endl;
    std::cout << answer << std::endl;
}

int main() {
    return 0;
}
