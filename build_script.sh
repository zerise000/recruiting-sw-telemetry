mkdir -p logs 
mkdir -p csv


if [ ! -d build ]; then
  mkdir build
  cd build
  cmake ..
else
  if [ ! -z "./logs" ] && [ ! -z "./csv" ]; then
	  rm "./logs"/*
	  rm "./csv"/*
  fi

  cd build
fi

make -j$(nproc)
