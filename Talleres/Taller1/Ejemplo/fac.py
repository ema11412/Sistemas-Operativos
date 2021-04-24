from time import time

def main(i,result):
    if i==1:
        return result
    else:  
        return main(i-1,result*i)

inicio = time()       
main(300,1)
fin = (time() - inicio)

print(fin)

