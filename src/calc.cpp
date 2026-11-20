#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace {

constexpr double kPi = 3.14159265358979323846;

double deg_to_rad(double deg) {
    return deg * (kPi / 180.0);
}

void print_usage() {
    std::cerr
        << "Usage:\n"
        << "  calc add <a> <b>\n"
        << "  calc sub <a> <b>\n"
        << "  calc mul <a> <b>\n"
        << "  calc div <a> <b>\n"
        << "  calc pow <base> <exp>\n"
        << "  calc sqrt <x>\n"
        << "  calc exp <x>\n"
        << "  calc ln <x>\n"
        << "  calc log10 <x>\n"
        << "  calc sin_deg <deg>\n"
        << "  calc cos_deg <deg>\n"
        << "  calc tan_deg <deg>\n"
        << "  calc sin_rad <rad>\n"
        << "  calc cos_rad <rad>\n"
        << "  calc tan_rad <rad>\n";
}

bool parse_double(const char* raw, double& out) {
    if (raw == nullptr || *raw == '\0') {
        return false;
    }
    char* end = nullptr;
    out = std::strtod(raw, &end);
    return end != raw && *end == '\0' && std::isfinite(out);
}

}  // namespace

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    const std::string op = argv[1];
    double a = 0.0;
    if (!parse_double(argv[2], a)) {
        std::cerr << "Error: invalid numeric argument '" << argv[2] << "'\n";
        return 1;
    }

    if (op == "sqrt") {
        if (argc != 3 || a < 0.0) {
            std::cerr << "Error: sqrt requires a non-negative number\n";
            return 1;
        }
        std::cout << std::sqrt(a) << "\n";
        return 0;
    }

    if (op == "exp") {
        if (argc != 3) {
            print_usage();
            return 1;
        }
        const double res = std::exp(a);
        if (!std::isfinite(res)) {
            std::cerr << "Error: exp overflow\n";
            return 1;
        }
        std::cout << res << "\n";
        return 0;
    }

    if (op == "ln") {
        if (argc != 3 || a <= 0.0) {
            std::cerr << "Error: ln requires a positive number\n";
            return 1;
        }
        std::cout << std::log(a) << "\n";
        return 0;
    }

    if (op == "log10") {
        if (argc != 3 || a <= 0.0) {
            std::cerr << "Error: log10 requires a positive number\n";
            return 1;
        }
        std::cout << std::log10(a) << "\n";
        return 0;
    }

    if (op == "sin_deg" || op == "cos_deg" || op == "tan_deg") {
        if (argc != 3) {
            print_usage();
            return 1;
        }
        const double rad = deg_to_rad(a);
        if (op == "sin_deg") {
            std::cout << std::sin(rad) << "\n";
        } else if (op == "cos_deg") {
            std::cout << std::cos(rad) << "\n";
        } else {
            if (std::cos(rad) == 0.0) {
                std::cerr << "Error: tan_deg undefined at angle\n";
                return 1;
            }
            std::cout << std::tan(rad) << "\n";
        }
        return 0;
    }

    if (op == "sin_rad" || op == "cos_rad" || op == "tan_rad") {
        if (argc != 3) {
            print_usage();
            return 1;
        }
        if (op == "sin_rad") {
            std::cout << std::sin(a) << "\n";
        } else if (op == "cos_rad") {
            std::cout << std::cos(a) << "\n";
        } else {
            if (std::cos(a) == 0.0) {
                std::cerr << "Error: tan_rad undefined at angle\n";
                return 1;
            }
            std::cout << std::tan(a) << "\n";
        }
        return 0;
    }

    if (argc != 4) {
        print_usage();
        return 1;
    }

    double b = 0.0;
    if (!parse_double(argv[3], b)) {
        std::cerr << "Error: invalid numeric argument '" << argv[3] << "'\n";
        return 1;
    }

    if (op == "add") {
        std::cout << (a + b) << "\n";
        return 0;
    }
    if (op == "sub") {
        std::cout << (a - b) << "\n";
        return 0;
    }
    if (op == "mul") {
        std::cout << (a * b) << "\n";
        return 0;
    }
    if (op == "div") {
        if (b == 0.0) {
            std::cerr << "Error: division by zero\n";
            return 1;
        }
        std::cout << (a / b) << "\n";
        return 0;
    }
    if (op == "pow") {
        const double res = std::pow(a, b);
        if (!std::isfinite(res)) {
            std::cerr << "Error: pow out of range\n";
            return 1;
        }
        std::cout << res << "\n";
        return 0;
    }

    std::cerr << "Error: unknown operation '" << op << "'\n";
    print_usage();
    return 1;
}
