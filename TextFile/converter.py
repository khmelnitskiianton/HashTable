counter_spaces = 0
with open('TextFile/gamlet.txt',     'r', encoding="ascii") as file_src:
    buffer_src = file_src.read()
with open('TextFile/dictionary.txt', 'w', encoding="ascii") as file_dst:
    for ch in buffer_src:
        if ch.isalnum() == True:
            counter_spaces = 0
            file_dst.write(ch)
            continue
        if ch.isspace() == True:
            if (counter_spaces == 1):
                continue
            file_dst.write('\n')
            counter_spaces += 1