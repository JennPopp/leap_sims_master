#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec 19 13:30:05 2022

@author: jenpopp
"""
import matplotlib.pyplot as plt 
import numpy as np
from textwrap import wrap
import seaborn as sns
from helmholtzcolors import get_blues

def plot_full_question(s,Q_lab,outpath,logAx=False, dispPercent=False, noNA=False, save=True):
    fig, ax = plt.subplots(dpi=150,figsize=(12,10))
    counts = s.count(Q_lab,labels=True)
    if noNA : 
        counts = counts.drop("No Answer")
    counts = counts.sort_values(by=counts.columns[0], ascending=False)
    Q = counts.columns[0]
    labels= list(counts.index.values)
    labels = [ '\n'.join(wrap(l, 40)) for l in labels ]
    
    blue_palette = get_blues(len(counts))
    p = sns.barplot(x=counts.iloc[:,0], y=labels, palette=blue_palette)
    #
    #p = counts.plot(kind='barh', legend=False, title = counts.columns[0], color=blue_palette)
    if logAx:
        p.set_xscale('log')
        
    if dispPercent:
        barlabs= [f"{round(v,1)} %" for v in np.divide(counts.iloc[:,0],len(s.get_responses(Q_lab).index))*100]
        p.bar_label(p.containers[0],labels=barlabs, fontsize=16)
        
    else:
        p.bar_label(p.containers[0],fontsize=16)
        
        
    p.set_xlabel("Number of Responses", fontsize=16)
    #p.set_title('\n'.join(wrap(Q, 80)),fontsize=14)
    p.tick_params(labelsize=13)
    plt.text(.99, .05, f" N = {len(s.get_responses(Q_lab).index)}", ha='right', va='top', transform=ax.transAxes,fontsize=16)
    fig.tight_layout()
    if save:
        plt.savefig(outpath)


def get_PA_counts(s,Q_lab, subject): 
    Q=s.count(Q_lab,labels=True).columns[0]
    N=len(s.get_responses(Q_lab).index)
    cF = s.count(Q_lab, labels=True)
    idk = cF.at["I don't know",Q]
    na = cF.at["I don't want to answer this question", Q]
    no = cF.at[subject, Q]
    yes = N-no -idk - na
    c2 = [yes, no, idk+na]
    return c2

def bar_PA_counts(Q, choices, labels, colors, barWidth, outpath, plotTitle=True):
    fig, ax = plt.subplots(dpi = 300, figsize=(9,5.8))
    for i, choice in enumerate(choices.keys()):
        if i==0:
            r = np.arange(len(labels))
        else:
            r = [x +i*(barWidth+0.02) for x in np.arange(len(labels))]

        b = plt.bar(r, choices[choice], color=colors[i], width=barWidth, edgecolor='white', label='\n'.join(wrap(choice, 15)))
        
        barlabs= [f"{round(v,1)} %" for v in np.divide(choices[choice],sum(choices[choice]))*100]
        ax.bar_label(b,labels=barlabs, fontsize=16)

    labels = ['\n'.join(wrap(l, 15)) for l in labels]
    
    if len(choices) == 2: 
        plt.xticks([r + barWidth/2 for r in range(len(labels))], labels, fontsize=16)
    elif len(choices) == 3: 
        plt.xticks([r + barWidth for r in range(len(labels))], labels, fontsize=16)
        
    # Create legend & Show graphic
    plt.legend(fontsize=16)
    plt.yscale('log')
    
    if plotTitle:
        plt.title('\n'.join(wrap(Q, 80)),fontsize=16)
    plt.ylabel("Number of Responses",fontsize=16)
    plt.tight_layout()
    plt.savefig(outpath )
    
    
    return ax
    
    
def bar_PA_counts_subplot(ax, Q, choices, labels, colors, barWidth,Ylab=False):
    
    labels = ['\n'.join(wrap(l, 15)) for l in labels]
    for i, choice in enumerate(choices.keys()):
        if i==0:
            r = np.arange(len(labels))
        else:
            r = [x +i*(barWidth+0.03) for x in np.arange(len(labels))]

        b = ax.bar(r, choices[choice], color=colors[i], width=barWidth, edgecolor='white', label='\n'.join(wrap(choice, 25)))
        
        barlabs= [f"{round(v)} %" for v in np.divide(choices[choice],sum(choices[choice]))*100]
        ax.bar_label(b,labels=barlabs, fontsize=12)

    if len(labels) == 2: 
        ax.set_xticks([r + barWidth/2 for r in range(len(labels))], labels, fontsize=14)
    elif len(labels) == 3: 
        ax.set_xticks([r + barWidth for r in range(len(labels))], labels, fontsize=14)
     
    # Create legend & Show graphic
    ax.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='lower left',
                      ncol=2, mode="expand", borderaxespad=0., fontsize=12)
    plt.yscale('log')

    if Ylab:
        ax.set_ylabel("Number of Responses",fontsize=14)
   # plt.tight_layout()
        

def get_sh_counts(s,Q_lab): 
    Q=s.count(Q_lab,labels=True).columns[0]
    s_exp = s.query("F4_SQ002=='Y' or F4_SQ003=='Y' or F4_SQ004=='Y' or F4_SQ005=='Y' or F4_SQ006=='Y' or F4_SQ007=='Y'or F4_SQ008=='Y'  or F4_SQ009=='Y'") 
    yes = len(s_exp.get_responses("F4").index)
    cF = s.count(Q_lab, labels=True)
    idk = cF.at["I don't know",Q]
    na = cF.at["I don't want to answer this question", Q]
    no = cF.at["No, I have not been subject to sexual harassment", Q]
    c2 = [yes, no, idk+na]
    return c2    

#def get_CarDevCounts(s,Q_lab):
    

def get_bu_counts(s,Q_lab): 
    Q=s.count(Q_lab,labels=True).columns[0]
    s_exp = s.query("F6_SQ002=='Y' or F6_SQ003=='Y' or F6_SQ004=='Y' or F6_SQ005=='Y' or F6_SQ006=='Y' or F6_SQ007=='Y'or F6_SQ008=='Y'") 
    yes = len(s_exp.get_responses("F6").index)
    cF = s.count(Q_lab, labels=True)
    idk = cF.at["I don't know",Q]
    na = cF.at["I don't want to answer this question", Q]
    no = cF.at["No, I have not been subject to bullying", Q]
    c2 = [yes, no, idk+na]
    return c2 