test: ./build/test/piper_tests
	make -C build/
	./build/test/piper_tests

run: ./build/piper/piper
	make -C build/
	./build/piper/piper ./models/bunny.obj
