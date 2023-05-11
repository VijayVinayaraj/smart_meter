import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd


x=[]
y=[]
def animate(i):
    data =pd.read_csv('./log/data.csv')
    x = data['Time']
    y1 = data['Energy']
    y2 = data['AccEnergy']
    print(x)
    print(y1)
    print(y2)
    plt.cla()
    plt.plot(x,y1, label="energy")
    plt.plot(x,y2,label='current')    
    plt.legend()
    plt.xticks(rotation=45)



ani = animation.FuncAnimation(plt.gcf(), animate, interval=1000)
plt.show()