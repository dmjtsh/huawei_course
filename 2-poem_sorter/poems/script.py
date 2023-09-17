f = open('onegin.txt')
count = 0
for i in f:
    for j in i:
        count+=1
print(count)
