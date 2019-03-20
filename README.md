{% cq %} # Android Kernel Source For Xiaomi Redmi 4X (Nougat/MIUI) {% endcq %}

## Before Build
1.Setup Build Environment
```bash
sudo apt update && sudo install -y git ccache automake flex lzop bison \
gperf build-essential zip curl zlib1g-dev zlib1g-dev:i386 \
g++-multilib python-networkx libxml2-utils bzip2 libbz2-dev \
libbz2-1.0 libghc-bzlib-dev squashfs-tools pngcrush \
schedtool dpkg-dev liblz4-tool make optipng maven libssl-dev \
pwgen libswitch-perl policycoreutils minicom libxml-sax-base-perl \
libxml-simple-perl bc libc6-dev-i386 lib32ncurses5-dev \
x11proto-core-dev libx11-dev lib32z-dev libgl1-mesa-dev xsltproc unzip
```

2.Checkout Kernel Source
```bash
$ git clone --depth=1 https://github.com/DroidKali/santoni_kernel_nethunter.git
```

3.Download Build Toolchain
```bash
$ git clone https://bitbucket.org/UBERTC/aarch64-linux-android-4.9-kernel.git toolchain
$ export ARCH=arm64
$ export SUBARCH=arm64
$ export CROSS_COMPILE=${pwd}/toolchain/bin/aarch64-linux-android-
```

### For BUILD MIUI KERNEL
```bash
$ cd santoni_kernel_nethunter
$ mkdir out 
$ make O=out mrproper
```
```bash
make O=out santoni_defconfig ARCH=arm64 CROSS_COMPILE=${pwd}/toolchain/bin/aarch64-linux-android-
make -j$(nproc) O=out 2>&1 ARCH=arm64 CROSS_COMPILE=${pwd}/toolchain/bin/aarch64-linux-android-
```

### For BUILD Kali NetHunter KERNEL
```bash
chmod +x menuconfig.sh build.sh && ./menuconfig.sh
```

```bash
./build.sh
```
