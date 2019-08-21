import os
from sys import argv

def JetAlgoparseINP(input,data): 
    """
    Parse out Jet Algo input vector
    """
    Nt = 816 # Set total number of towers in card
    tpl = 18 # Number of towers per link
    etunit = 0.25 # GeV
    fname = input.replace('data/','')
    with open(input,'r') as f: links = [ link.replace('\n','') for link in f.readlines() ]

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
    Parse out RCT output vector

    3 Cluster per link
    Peak eta - 3 bit
    Peak phi - 3 bit
    Tower eta - 6 bit
    Tower phi - 4 bit
    Cluster Et - 16 bit
    
    + = used; - = reserved; 0 = un-used
            ++++++++|--------
    Link -> ++++++++|++++++++
            00000000|00000000
    
    """

    Nc = 500
    fname = input.replace('data/','')
    links = []
    with open(input,'r') as f:
        text = f.readlines()[3:5]
        for i,line in enumerate(text):
            bits = line.split()[1:]
            for j,bit in enumerate(bits):
                if i == 0: links.append([])
                if i != 0:
                    binary = format(int(bit[-8:],16),'032b')
                    links[j].append(
                        {
                            'pEta':int(binary[-3:],2),
                            'pPhi':int(binary[-6:-3],2),
                            'tEta':int(binary[-12:-6],2),
                            'tPhi':int(binary[-16:-12],2),
                            'cEt' :int(binary[-32:-16],2)
                        }
                    )
                binary = format(int(bit[-16:-8],16),'032b')
                links[j].append(
                    {
                        'pEta':int(binary[-3:],2),
                        'pPhi':int(binary[-6:-3],2),
                        'tEta':int(binary[-12:-6],2),
                        'tPhi':int(binary[-16:-12],2),
                        'cEt' :int(binary[-32:-16],2)
                    }
                )

    crystals = [0]*Nc
    nCluster = 0
    for link in links:
        for cluster in link:
            id = cluster['tPhi']*25 + cluster['tEta']*100 + cluster['pPhi'] + cluster['pEta']*5
            if not id < 500: continue
            crystals[id] = cluster['cEt'] if cluster['cEt'] != 0 else 0.5
            nCluster += 1
            if nCluster == 10: break
        if nCluster == 10: break
        
    data.append({'fname':fname,'links':links,'crystals':crystals})

######################################################################

data = []

for input in argv[1:]:
    if 'out' in input: JetAlgoparseOUT(input,data)
    if 'inp' in input: JetAlgoparseINP(input,data)
        
