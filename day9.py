import plotly.express as px

coords = []
with open("d9key.txt") as f:
    coords = [tuple(map(int, x.strip().split(','))) for x in f.readlines()]

y = [i[0] for i in coords]
x = [i[1] for i in coords]

fig = px.line(x=x,y=y)
fig.show()