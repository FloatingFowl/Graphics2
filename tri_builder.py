orig = []

with open("code_builder") as f:
    content = f.readlines()

orig = content

final = [None] * 25

final[19]=final[15]=final[4]=final[1]=orig[0]
final[20]=final[10]=final[7]=final[3]=orig[1]
final[21]=final[16]=final[13]=final[9]=orig[2]

final[22]=final[18]=final[14]=final[5]=orig[3]
final[23]=final[11]=final[6]=final[2]=orig[4]
final[24]=final[17]=final[12]=final[8]=orig[5]

for i in range(1,len(final)):
    print final[i],
