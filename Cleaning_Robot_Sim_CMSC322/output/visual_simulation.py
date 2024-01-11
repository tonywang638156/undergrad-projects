import pandas as pd
import numpy as np
import plotly.graph_objects as go
import plotly.express as px

def drawSmallRoom(fig, roomIndex):
    lowerX = 999
    lowerY = 17 + (int(roomIndex / 5) * 21)
    if (roomIndex % 5 == 1):
        lowerX = 4
    elif (roomIndex % 5 == 2):
        lowerX = 8
    elif (roomIndex % 5 == 3):
        lowerX = 12
    elif (roomIndex % 5 == 4):
        lowerX = 16
    else:
        lowerX = 0
    fig.add_shape(type="rect",
            x0=lowerX, y0=lowerY, x1=(lowerX+3), y1=(lowerY+3),
            line=dict(color="#6d59c9", width=5,),
            #fillcolor='#ff7f0e',
    )
    fig.add_trace(go.Scatter(
        x=[lowerX+1.5],
        y=[lowerY+1.5],
        text=[f"S{roomIndex}"],
        mode="text",
    ))

def drawMedianRoom(fig, roomIndex):
    lowerX = 999
    lowerY = 10 + (int(roomIndex / 3) * 21)
    if (roomIndex % 3 == 1):
        lowerX = 7
    elif (roomIndex % 3 == 2):
        lowerX = 14
    else:
        lowerX = 0
    fig.add_shape(type="rect",
            x0=lowerX, y0=lowerY, x1=(lowerX+6), y1=(lowerY+6),
            line=dict(color="#ff7fff", width=5,),
            #fillcolor='#ff7f0e',
    )
    fig.add_trace(go.Scatter(
        x=[lowerX+3],
        y=[lowerY+3],
        text=[f"M{roomIndex}"],
        mode="text",
    ))

def drawLargeRoom(fig, roomIndex):
    lowerX = 10
    lowerY = int(roomIndex / 2) * 21
    if (roomIndex % 2 == 0):
        lowerX = 0
    fig.add_shape(type="rect",
            x0=lowerX, y0=lowerY, x1=(lowerX+9), y1=(lowerY+9),
            line=dict(color="#ba642f", width=5,),
            #fillcolor='#ff7f0e',
    )
    fig.add_trace(go.Scatter(
        x=[lowerX+4.5],
        y=[lowerY+4.5],
        text=[f"L{roomIndex}"],
        mode="text",
    ))

df_room = pd.read_csv('room_count.csv')
numOfSamllRoom = df_room.loc[0,' small']
numOfMedianRoom = df_room.loc[0,' medium']
numOfLargeRoom = df_room.loc[0,'large']

df = pd.read_csv('simulationInput.csv', header = None)
df = df.rename(columns = {0:'x', 1:'y', 2:'t', 3:'id'})


fig = go.Figure()
fig = px.scatter(df, x = 'x', y = 'y', animation_frame = 't', animation_group = 'id',  range_x=[-1,23], range_y=[-1,66],
         template = 'simple_white', color = 'id')#, size = 'type'
for i in range(numOfSamllRoom):
    drawSmallRoom(fig,i)
for i in range(numOfMedianRoom):
    drawMedianRoom(fig,i)
for i in range(numOfLargeRoom):
    drawLargeRoom(fig,i)
    
fig.update_xaxes(range=[-0.5, 35], showgrid=False)
fig.update_yaxes(range=[-0.5, 35])
fig.show()