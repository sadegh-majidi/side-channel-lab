
all: attack_prime_probe attack_flush_reload cov_prime_probe cov_flush_reload

attack_prime_probe:
	g++ -g attack_prime_probe.cpp aes_lib.cpp prime_probe.cpp -o prime_probe

attack_flush_reload:
	g++ -g attack_flush_reload.cpp aes_lib.cpp -o flush_reload

cov_prime_probe:
	g++ -g covert_prime_probe.cpp prime_probe.cpp -o cov_prime_probe

cov_flush_reload:
	g++ -g covert_flush_reload.cpp -o cov_flush_reload
clean:
	rm -f prime_probe
	rm -f flush_reload
	rm -f cov_prime_probe
	rm -f cov_flush_reload
