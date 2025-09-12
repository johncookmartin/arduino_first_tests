int increment(int value, int max, int min) {
	if (value == max) {
		return min;
	} else {
		return value + 1;
	}
}

int decrement(int value, int max, int min) {
	if (value == min){
		return max;
	} else {
		return value - 1;
	}
}