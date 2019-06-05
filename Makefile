make: *.cpp
	@g++ -o main *.cpp -std=c++11
run:
	@./main MOCK_DATA.csv
clean:
	@rm main
