#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>

class NightTestSuite {
public:
    bool verify_file_structures() {
        std::vector<std::string> critical_files = {
            "api/rest_core.py",
            "docs/generate_man.py",
            "build/build_pipeline.py",
            "packages/repo_mirror.py"
        };
        for (const auto& path : critical_files) {
            std::ifstream f(path);
            if (!f.good()) return false;
        }
        return true;
    }

    int execute_assertions() {
        assert(verify_file_structures() == true);
        return 0;
    }
};

int main() {
    NightTestSuite suite;
    return suite.execute_assertions();
}
