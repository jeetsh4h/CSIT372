import matplotlib.pyplot as plt

hashes = []
names = []
with open('hash_probe.txt', 'r') as f:
    while True:
        line = f.readline()
        if not line:
            break

        hash, name = line.split(",")
        if name.strip() in names:
            continue

        hashes.append(int(hash))
        names.append(name.strip())

# Count the frequency of each hash value
hash_freq = {}
for hash_val in hashes:
    if hash_val in hash_freq:
        hash_freq[hash_val] += 1
    else:
        hash_freq[hash_val] = 1

# Prepare the data for plotting
x = list(range(1009))  # All possible hash values between 0 and 1008
y = [hash_freq.get(hash_val, 0) for hash_val in x]  # Frequency of each hash value

print(len(y) - (y.count(1) + y.count(0)))

# Create the line graph
# plt.plot(x, y)
# plt.xlabel('Hash Value')
# plt.ylabel('Frequency')
# plt.title('Hash Value Frequency')
# plt.show()