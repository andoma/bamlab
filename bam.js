
const version = 'testversion';

const run_container = debootstrap({
  sources: [
    "http://archive.ubuntu.com/ubuntu noble main restricted universe",
    "http://archive.ubuntu.com/ubuntu noble-updates main restricted universe",
    "http://archive.ubuntu.com/ubuntu noble-security main restricted universe"
  ]
});

const build_container = from(run_container);

build_container.apt([
  "make",
  "ccache",
  "build-essential"]);

for(name of ["test1", "test2", "test3"]) {
  const stage = from(build_container);
  stage.build(`mkdir /build && cd /workspace && gcc -O2 -Wall -Werror -fdiagnostics-format=sarif-stderr -o /build/test ${name}/${name}.c`, {
    paths: [name],
    report:"build",
  });
  stage.verifyElf("/build/test");
  stage.shell("/build/test");
}
