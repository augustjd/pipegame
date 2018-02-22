run: ./build/piper
	make -C build/
	./build/piper ./models/bunny.obj


test: ./build/test/piper_tests
	make -C build/
	./build/test/piper_tests
