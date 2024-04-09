counter_spaces = 0
with open('Processing/gamlet.txt',     'r', encoding="ascii") as file_src:
    buffer_src = file_src.read()
with open('Processing/dictionary.txt', 'w', encoding="ascii") as file_dst:
    for ch in buffer_src:
        if ch.isalnum() == True:
            counter_spaces = 0
            file_dst.write(ch)
            continue
        if ch.isspace() == True:
            if (counter_spaces >= 1):
                continue
            file_dst.write('\n')
            counter_spaces += 1
        if (ch == '-'):
            if (counter_spaces >= 1):
                continue
            file_dst.write('\n')
            counter_spaces += 1

x = []
with open('Processing/dictionary.txt', 'r', encoding="ascii") as file_read:
    y = file_read.readlines()
    for line in y:
        word = line.strip().split()[0]
        #print(word)
        x.append(len(word))
    print(max(x))
    for line in y:
        word = line.strip().split()[0]
        if (len(word) == max(x)):
            print(word)