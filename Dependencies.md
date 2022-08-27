# Compiler 
## Clang

### Linux
```bash
sudo apt-get install clang
```

### Mac
```bash
xcode-select -install
```

# Packages

## Juce

### Linux
```
sudo apt-get install libasound2-dev libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev  \
    libfreetype6-dev \
    libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev mesa-common-dev
```

## Boost

### Mac \ Linux

```
wget https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.tar.bz2
tar --bzip2 -xf boost_1_79_0.tar.bz2

rm boost_1_79_0.tar.bz2
cd boost_1_79_0

./bootstrap.sh --prefix=/usr/~ --with-toolset=clang --with-python-version=3.10
./b2
sudo ./b2 install
```
