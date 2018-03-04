orig = []

with open("code_builder") as f:
    content = f.readlines()

orig = content

final = [None] * 19

final[16]=final[13]=final[11]=final[1]=orig[0]
final[15]=final[4]=final[2]=orig[1]
final[18]=final[14]=final[7]=final[5]=orig[2]
final[17]=final[10]=final[8]=orig[3]

final[3]=final[6]=final[9]=final[12]=orig[4]

for i in range(1,len(final)):
    print final[i],
