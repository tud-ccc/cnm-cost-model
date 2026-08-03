def comment(file_name, position):
    f = open(file_name, "r")
    new_file_lines = []
    next_line_is_the_line = False

    for line in f:
        if next_line_is_the_line:
            next_line_is_the_line = False
            if ".LBB" in line:
                new_file_lines.append("\tjump" + line.split(",")[-1])
        else:
            if position in line:
                next_line_is_the_line = True
            new_file_lines.append(line)
    f.close()
    f = open(file_name, "w")
    for line in new_file_lines:
        f.write(line)
    f.close()


def comments(file_name, positions):
    f = open(file_name, "r")
    new_file_lines = []
    next_line_is_the_line = False

    for line in f:
        if next_line_is_the_line:
            next_line_is_the_line = False
        else:
            canWriteLine = True
            for position in positions:
                if position[0] in line and position[1] == False:
                    next_line_is_the_line = True
                elif position[0] in line and position[1] == True:
                    canWriteLine = False
            if canWriteLine:
                new_file_lines.append(line)
    f.close()
    f = open(file_name, "w")
    for line in new_file_lines:
        f.write(line)
    f.close()
