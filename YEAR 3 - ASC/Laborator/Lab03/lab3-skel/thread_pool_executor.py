import random
from concurrent.futures import ThreadPoolExecutor

DNA_SEQ = 'ATGC'
DNA_FOUND = "DNA sequence found in sample {}."
DNA_NOT_FOUND = "DNA sequence not found."
samples_searched = []

def generate_DNA_samples():
	result = []

	for i in range(0, 100):
		sample = ""
		for i in range (0, 10000):
			sample = sample + (random.choice(DNA_SEQ))

		result.append(sample)

	return result

def find_seq(samples, seq, sample_index):
	if sample_index not in samples_searched:
		samples_searched.append(sample_index)

	random_sample = samples[sample_index]

	result = random_sample.find(seq)

	if result != -1:
		return DNA_FOUND.format(sample_index)
	else:
		return DNA_NOT_FOUND

if __name__ == "__main__":
	random.seed(1)
	samples = generate_DNA_samples()
	seq = "AGTGCCCCGATAACGCAGGACACCA"

	results = []

	with ThreadPoolExecutor(max_workers = 30) as executor: 
		for i in range(0, 100):
			results.append(executor.submit(find_seq, samples, seq, i))

	for result in results:
		print(result.result())