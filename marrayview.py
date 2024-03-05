import tkinter as tk
from tkinter import messagebox
from tkinter.filedialog import askopenfilename
from tkinter.filedialog import asksaveasfilename
from tkinter import *
import subprocess
import shutil
import os
from PIL import Image 
from PIL import ImageTk
import PIL


images=None
ccanvas=None
def msgbox(msgs:str,color:str):
    Window = tk.Toplevel(bg=color)

    
    filename = tk.filedialog.askopenfilename(title="load file")
    
    f1 = open(filename,"br+")
    sss1:str=str(f1.read(9))

    sss1:str=sss1[:]
    sss1=sss1.replace("b'","")
    sss1=sss1.replace("\\x00'","\0")
    print(sss1)
    if sss1!="MapArray\0":
        f1.close()
        print("return title")
        return None
    # obtém as dimensões da imagem
    Width:str= str(f1.read(4))
    Width=Width.replace("\\x","")
    Width=Width.replace("b'","")
    Width=Width.replace("'","")

    Width:int=int(Width[0:2],16)+int(Width[2:4],16)*256+int(Width[4:6],16)*256*256+int(Width[6:8],16)*256*256*256
    Height:str= str(f1.read(4))
    Height=Height.replace("\\x","")
    Height=Height.replace("b'","")
    Height=Height.replace("'","")

    Height:int=int(Height[0:2],16)+int(Height[2:4],16)*256+int(Height[4:6],16)*256*256+int(Height[6:8],16)*256*256*256

    m32:str=str(f1.read(4))
    m32=m32.replace("\\x","")
    m32=m32.replace("b'","")
    m32=m32.replace("'","")
    print(m32)
    m32:int=int(m32[0:2],16)+int(m32[2:4],16)*256+int(m32[4:6],16)*256*256+int(m32[6:8],16)*256*256*256
    print(Width)
    print(Height)   
    if m32!=32 and m32!=0:
        f1.close()
        print(m32)
        print("return"+str(m32))
        return None
    # percorre todos os pixels da imagem e muda a cor azul para vermelho
    canvas = tk.Canvas(Window, width=Width, height=Height, bg=color)
    for y in range(Height):
        for x in range(Width):
            # obtém o valor RGB do pixel atual
            rgb= f1.read(4)
            r=rgb[0]
            g=rgb[1]
            b=rgb[2]
            
            # verifica se o pixel é azul (R=0, G=0, B=255) e o substitui por vermelho (R=255, G=0, B=0)
            #if r == 0 and g == 0 and b == 255:
            s=hex(r+g*256+b*256*256)
            s=s.replace("0x","000000")
            s1=len(s)
            s2=s1-6
            s="#"+s[s2:s1]
            #print(s)
            canvas.create_rectangle((x, y),(x+1,y+1),outline="", fill=s)
    
    
    canvas.pack()
    
    
class BareboneBuilder:
    def __init__(self, root):
        i:int=0
        self.root = root
        self.root.title("")

        # Janela amarela
        self.root.configure(bg='blue')
        # Botões
        self.run_button = tk.Button(self.root, text="load file", command=self.build_kernel)
        self.run_button.pack(pady=5)
       

    def build_kernel(self):
        msgbox("hello world....",'blue') 
       
        
        
       






if __name__ == "__main__":
    root = tk.Tk()
    builder = BareboneBuilder(root)
    root.mainloop()
