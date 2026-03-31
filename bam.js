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

const libcore = from(toolchain).name("libcore");
libcore.cmake({
  sourceDir: "libcore",
  buildDir: "/build/libcore",
  generator: "Ninja",
  buildType: "Release",
  test: true,
  sarif: true,
  paths: ["libcore"],
});

// Install libcore headers + library into rootfs
libcore.build("cmake --install /build/libcore --prefix /usr/local", {
  paths: ["libcore"],
});

// --- Build analyzer app ---

const analyzer = from(libcore).name("analyzer");
analyzer.cmake({
  sourceDir: "analyzer",
  buildDir: "/build/analyzer",
  generator: "Ninja",
  buildType: "Release",
  sarif: true,
  paths: ["analyzer"],
});

// Quick smoke test
analyzer.build("echo 'hello world hello' | /build/analyzer/analyzer --freq", {
  paths: ["analyzer"],
});

// --- Build calculator app ---

const calculator = from(libcore).name("calculator");
calculator.cmake({
  sourceDir: "calculator",
  buildDir: "/build/calculator",
  generator: "Ninja",
  buildType: "Release",
  sarif: true,
  paths: ["calculator"],
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
