f = open('onegin.txt')
count = 0
for i in f:
    if i == 'A bastard fame prohibited?\n':
        count+=1
print(count)
