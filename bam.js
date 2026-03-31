// Build environment: Ubuntu with cmake, ninja, g++
const base = debootstrap({
  sources: [
    "http://archive.ubuntu.com/ubuntu noble main restricted universe",
    "http://archive.ubuntu.com/ubuntu noble-updates main restricted universe",
  ]
});

const toolchain = from(base);
toolchain.apt(["build-essential", "cmake", "ninja-build"]);

// --- Build libcore (shared library + tests) ---

const libcore = from(toolchain);
libcore.build("cmake -G Ninja -S /workspace/libcore -B /build/libcore -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fdiagnostics-format=sarif-stderr", {
  paths: ["libcore"],
  sarif: true,
  report: "libcore",
});
libcore.build("ninja -C /build/libcore", {
  paths: ["libcore"],
  sarif: true,
  report: "libcore",
});
libcore.build("ctest --test-dir /build/libcore --output-on-failure", {
  paths: ["libcore"],
  report: "libcore",
});

// Install libcore headers + library into rootfs
libcore.build("cmake --install /build/libcore --prefix /usr/local", {
  paths: ["libcore"],
});

// --- Build analyzer app ---

const analyzer = from(libcore);
analyzer.build("cmake -G Ninja -S /workspace/analyzer -B /build/analyzer -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fdiagnostics-format=sarif-stderr", {
  paths: ["analyzer"],
  sarif: true,
  report: "analyzer",
});
analyzer.build("ninja -C /build/analyzer", {
  paths: ["analyzer"],
  sarif: true,
  report: "analyzer",
});

// Quick smoke test
analyzer.build("echo 'hello world hello' | /build/analyzer/analyzer --freq", {
  paths: ["analyzer"],
});

// --- Build calculator app ---

const calculator = from(libcore);
calculator.build("cmake -G Ninja -S /workspace/calculator -B /build/calculator -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fdiagnostics-format=sarif-stderr", {
  paths: ["calculator"],
  sarif: true,
  report: "calculator",
});
calculator.build("ninja -C /build/calculator", {
  paths: ["calculator"],
  sarif: true,
  report: "calculator",
});

// Quick smoke test
calculator.build("/build/calculator/calculator '2 + 3 * 4' '(10 - 2) / 4'", {
  paths: ["calculator"],
});

// --- Final images: copy just the binaries into a minimal runtime ---

const runtime = from(base);

const analyzer_img = from(runtime);
analyzer_img.copyFrom(analyzer, "/build/analyzer/analyzer", "/usr/local/bin/analyzer");
analyzer_img.verifyElf("/usr/local/bin/analyzer");
analyzer_img.entrypoint("/usr/local/bin/analyzer");

const calculator_img = from(runtime);
calculator_img.copyFrom(calculator, "/build/calculator/calculator", "/usr/local/bin/calculator");
calculator_img.verifyElf("/usr/local/bin/calculator");
calculator_img.entrypoint("/usr/local/bin/calculator");
