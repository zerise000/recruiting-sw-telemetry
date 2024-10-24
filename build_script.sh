if [ ! -d build ]; then
  mkdir -p build
  cd build
  cmake ..
else
  cd build
  rm sessions/*
  make -j$(nproc)
fi

