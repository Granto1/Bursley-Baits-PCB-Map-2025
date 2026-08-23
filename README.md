# Introduction
I've been fascinated with public transit since my infant years riding the Toronto Subway with my dad. Public transit served as a one of the largest influences in my learning of Chinese as I rode the Beijing Subway and Bus systems with my grandparents. Of course, key in this was being able to read the transit maps and knowing where to go.

In Fall of 2025, entering my sophomore year of my Computer Engineering undergraduate degree and my second semester of the Michigan Hackers Embedded Subteam, I stumbled across a Reddit post that served as the catalyst for this project, a PCB map of an Australian Tram system. I have since lost the Reddit post, but I'm sure you'll find many more similar examples.

As an Engineering student at the University of Michigan, I can attest that many folks ignore the existance of the Northwoods, Diag-to-Diag, and sometimes the Commuter North buses in favor of jamming into the Bursley-Baits buses. As such, the BB has become a lifeline for folks trying to dart up to North and vice-versa to Central Campus.

So during project proposals for the semester, I thought, why not make a transit map for the Bursley Baits Bus line?

# Overview
This version was heavily simplified, as I was getting used to PCB design. A Xiao ESP32 controls a chain of ~60 Neopixel Addressable LEDs. A PCB with the text directly taken off the official Bursley Baits transit map was designed. Data is pulled from the Magic-Bus API which gives a rough location of the buses as they move around. For simplicity, and given some construction detours that were occuring at the time, we simply set out LEDs at rough geographic coordinates and distance formula-ed the bus coordinate with LED coordinates to turn them on.

![Official Bursley Baits Map](/images/BursleyBaits.png)
Fig 1. Official Transit Map

![PCB CAD Image](/images/CAD.png)
Fig 2. PCB in Altium

![PCB in operation, can you guess where this image was taken?](/images/Working.png)
Fig 3. PCB in Operation

Due to some Git issues with Altium, I have since lost the design files for the original PCB. I do, however, have the production files, which are included in the PCB folder.

# Acknowledgements
The code for this repository was previously hosted on a private repository by one of our team members. This repository aims to move the contents to public in anticipation for a second version that I'm working on. 

I would like to note the majority software contributions from [Henry LeCompte](https://github.com/HenryLeC) and [Luke Birchmeier](https://github.com/luke-birchmeier). They worked on the ESP32 interfacing with Magic-Bus API code, while I worked on integrating everything on the designed PCB.

# What's Next?
Working on a version that does not rely on addressable neopixels. Will start a new repository when the layout is complete.