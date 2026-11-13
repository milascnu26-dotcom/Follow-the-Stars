import turtle
import random

# Big Dipper er simplified position (x, y)
stars = [(-150, -20), (-90, 0), (-40, 20), (10, 30), (20, -30), (90, -30), (95, 40)]

screen = turtle.Screen()
screen.bgcolor("black")
t = turtle.Turtle()
t.hideturtle()
t.speed(3)
t.color("skyblue")

# Dhire dhire star gulo line diye joro
t.penup()
t.goto(stars[0])
t.pendown()
for x, y in stars[1:] + [stars[3]]:
    t.goto(x, y)

# Twinkle effect
for _ in range(30):
    x, y = random.choice(stars)
    t.penup()
    t.goto(x, y)
    t.dot(random.randint(8, 16), random.choice(["gold", "white", "yellow"]))
turtle.done()
