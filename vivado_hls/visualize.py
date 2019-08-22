import os
import sys
import numpy as np
import parseTV as tv
from ROOT import *

gROOT.SetBatch(1)

def getTowerPos(i):
    iphi = i%4 + 4*(i/136) + 1
    ieta = (i/4)%34 + 1
    return iphi,ieta

maxEta = 34
maxPhi = 24

for input in tv.data:
    x_list = [];
    y_list = [];
    weights = [];

    if 'jets' in input:   type = 'jets'
    if 'towers' in input: type = 'towers'
    for id,info in enumerate(input[type]):
        if info == 0: continue
        if type == 'towers':
            iphi,ieta = getTowerPos(id)
            et = info * 0.25
        if type == 'jets':
            iphi = info['phi']
            ieta = info['eta']
            et = info['et'] * 0.25
        x_list.append(iphi - 0.5)
        y_list.append(ieta - 0.5)
        weights.append(et)

    area = 1000000
    # a = xy; r = x/y
    # a*r = x^2;
    # x = sqrt(a*r)
    # y = sqrt(a/r)
    
    width = int((area*maxPhi/float(maxEta))**0.5)
    height = int((area*maxEta/float(maxPhi))**0.5)
        
    c = TCanvas("c", "canvas",width,height);
    gStyle.SetOptStat(0);
    gStyle.SetLegendBorderSize(0);
    c.SetRightMargin(0.15);
    c.SetTopMargin(0.01)
    c.SetBottomMargin(0.05)
    #c.SetLogy();
    #c.cd();
        
    gct = TH2F("tv",';#phi;#eta',maxPhi,0,maxPhi,maxEta,0,maxEta)
    for x,y,w in zip(x_list,y_list,weights): gct.Fill(x,y,w)

    gct.Draw("COLZ TEXT")
    gct.GetZaxis().SetTitle('E_{T} GeV')
    gct.GetZaxis().CenterTitle()
    gct.GetXaxis().SetTitleOffset(0.5)
    gct.GetXaxis().SetNdivisions(maxPhi/5)
    gct.GetXaxis().SetLabelOffset(999)
    gct.GetYaxis().SetNdivisions(maxEta/10)
    gct.GetYaxis().SetLabelOffset(999)
        
    lines = []
    axis = []
    for x in range(0,maxPhi):
        axis.append(TLatex(x+0.3,-0.8,str(x+1)))
        axis[-1].SetTextSize(0.03)
        axis[-1].Draw()
        lines.append(TLine(x,0,x,maxEta))
        if x%4 == 0: lines[-1].SetLineStyle(9)
        else: lines[-1].SetLineStyle(3)
        lines[-1].Draw()
    for y in range(0,maxEta):
        axis.append(TLatex(-0.9,y+0.2,str(y+1)))
        axis[-1].SetTextSize(0.03)
        axis[-1].Draw()
        lines.append(TLine(0,y,maxPhi,y))
        if y%17 == 0: lines[-1].SetLineStyle(0)
        else: lines[-1].SetLineStyle(3)
        lines[-1].Draw()
    if type == 'jets':
        boxes = []
        for (phi,eta) in zip(x_list,y_list):
            phi += 0.5; eta += 0.5
            box = TBox(phi - 5,eta - 5,phi + 4,eta + 4)
            box.SetFillStyle(0)
            box.SetLineColor(kRed)
            box.SetLineWidth(2)
            box.Draw('same')
            boxes.append(box)

    label="JetAlgo"
    outdir = '~/public_html/Trigger/'+label+'/HLS_Plots/'
    c.SaveAs(outdir+input['fname'].replace('.txt','.png'))



    
