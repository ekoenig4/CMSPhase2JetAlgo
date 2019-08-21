
def getTowerIndex(iphi,ieta):
  index = 136*( (iphi-1)/4 ) + (iphi-1)%4 + (ieta-1)*4;
  channel = index/18;
  bit = index%18;
  return index,channel,bit

for iphi in (1,2,3,4,5):
    for ieta in (1,2):
        print iphi,ieta,getTowerIndex(iphi,ieta)
