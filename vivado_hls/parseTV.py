import os
from sys import argv

def JetAlgoparseINP(input,data): 
    """
    Parse out Jet Algo input vector
    """
    Nt = 816 # Set total number of towers in card
    tpl = 18 # Number of towers per link
    etunit = 1 # GeV
    fname = input.replace('data/','')
    links = []
    with open(input,'r') as f:
        # Take only the first 3 lines of each link
        text = f.readlines()[3:6]
        for i,line in enumerate(text):
            if i > 2: break
            bits = line.split()[1:]
            for j,bit in enumerate(bits):
                if i == 0: links.append('')
                links[j] = bin( int(bit,16) )[2:].zfill(64) + links[j]
    # Create tower Et list 
    towers = []
    for link in links:
        for bit in range(18):
            bitLo = len(link) - bit*10
            bitHi = bitLo - 10
            towers.append( int( link[bitHi:bitLo],2 ) )
    ##########################################################
    data.append({'fname':fname,'links':links,'towers':towers})
            
######################################################################

def JetAlgoparseOUT(input,data):
    """
    Parse out Jet Algo output vector

    1 Jet per Link
    Peak phi - 6 bit
    Peak eta - 6 bit
    Jet Et - 10 bit
    """
        
    Nt = 816 # Set total number of towers in card
    tpl = 18 # Number of towers per link
    etunit = 0.25 # GeV
    fname = input.replace('data/','')
    links = []
    with open(input,'r') as f:
        # Take only the first 3 lines of each link
        text = f.readlines()[3:6]
        for i,line in enumerate(text):
            if i > 2: break
            bits = line.split()[1:]
            for j,bit in enumerate(bits):
                if i == 0: links.append('')
                links[j] = bin( int(bit,16) )[2:].zfill(64) + links[j]

    # Create Jet list 
    jets = []
    for link in links:
        if int(link,2) == 0: continue
        jet = {}
        bitLo = len(link); bitHi = bitLo - 6
        jet['phi'] = int(link[bitHi:bitLo],2)
        bitLo = bitHi; bitHi = bitLo - 6
        jet['eta'] = int(link[bitHi:bitLo],2)
        bitLo = bitHi; bitHi = bitLo - 10
        jet['et'] = int(link[bitHi:bitLo],2)
        jets.append(jet)
    ##########################################################
    data.append({'fname':fname,'links':links,'jets':jets})

######################################################################

data = []

for input in argv[1:]:
    if 'out' in input: JetAlgoparseOUT(input,data)
    if 'inp' in input: JetAlgoparseINP(input,data)
        
