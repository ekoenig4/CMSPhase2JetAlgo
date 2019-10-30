
def getTowerEt(iphi,ieta,link_in):
  index = 136*( (iphi-1)/4 ) + (iphi-1)%4 + (ieta-1)*4;
  channel = index/18;
  bit = index%18;
  link = link_in[channel];
  start = len(link) - bit*10;
  et = link[start-10:start];
  return index,channel,bit,link,et;
def getSuperTower(iphi,ieta):
  return (iphi-1)/3 + 8*( (ieta)/3 );
tv = 'data/jetalgo_tv_0_inp.txt'
with open(tv,'r') as f: link_in = [ link.replace('\n','') for link in f.readlines() ]

# print getTowerEt(17,19,link_in)
print getSuperTower(2,1)
