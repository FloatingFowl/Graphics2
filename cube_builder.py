orig = []

with open("code_builder") as f:
    content = f.readlines()

orig = content

final = [None] * 37

final[1] = final[4] = final[21] = final[25] = final[28] = orig[0]
final[3] = final[7] = final[10] = final[27] = orig[1]
final[9] = final[13] = final[16] = final[26] = final[30] = orig[2]

final[15] = final[19] = final[22] = final[29] = orig[3]
final[5] = final[20] = final[24] = final[31] = final[34] = orig[4]
final[2] = final[6] = final[11] = final[33] = orig[5]

final[8] = final[12] = final[17] = final[32] = final[36] = orig[6]
final[14] = final[18] = final[23] = final[35] = orig[7]

for i in range(1,len(final)):
    print final[i],
