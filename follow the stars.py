# =========================================================================
# SECTION 1: Screen and Window Setup - [Topic: Module Import]
# =========================================================================
import turtle
import time
import numpy as np              # Used for 5x vector distance calculations
import matplotlib.pyplot as plt # Used for generating the final star chart

screen = turtle.Screen()
screen.title("Kids Space Science: The Ultimate Twinkling Constellation Simulator!")
screen.bgcolor("#0B0F19")  
screen.setup(width=1000, height=750)


# =========================================================================
# SECTION 2: Custom Exception and Inheritance - [Topic: Inheritance & Exception]
# =========================================================================
class SpaceSimulationError(Exception):
    """Base exception class for all custom simulation errors."""
    pass

# InvalidStarDataError inherits from SpaceSimulationError [Topic: Inheritance]
class InvalidStarDataError(SpaceSimulationError):
    """Child exception raised when star data or coordinates are invalid."""
    pass


# =========================================================================
# SECTION 3: Star Blueprint Class Creation - [Topic: OOP Class & Composition]
# =========================================================================
class MissionStar:
    def __init__(self, x, y, name, color="yellow", size=14):
        self.x = x
        self.y = y
        self.name = name
        self.color = color
        self.size = size
        
        # Object Composition: Turtle object created inside the class
        self.pen = turtle.Turtle()
        self.pen.hideturtle()
        self.pen.speed(0)

    def draw(self, custom_color=None):
        # Using ternary operator [Topic: Conditional Expression]
        active_color = custom_color if custom_color else self.color
        
        self.pen.clear()  
        self.pen.color(active_color)
        self.pen.penup()
        self.pen.goto(self.x - (self.size/2), self.y)
        self.pen.pendown()
        
        self.pen.begin_fill()
        for _ in range(5):
            self.pen.forward(self.size)
            self.pen.right(144)
        self.pen.end_fill()
        
        if active_color != "#0B0F19":  
            self.pen.penup()
            self.pen.goto(self.x, self.y - 22)
            self.pen.color("#A0AEC0")
            self.pen.write(self.name, align="center", font=("Arial", 9, "bold"))  


# =========================================================================
# SECTION 4: Interface Text Control (UI Message Pen)
# =========================================================================
ui_pen = turtle.Turtle()
ui_pen.hideturtle()
ui_pen.penup()

def update_ui(message):
    ui_pen.clear()
    ui_pen.color("white")
    ui_pen.goto(0, 310)
    ui_pen.write(message, align="center", font=("Arial", 14, "bold"))  


# =========================================================================
# SECTION 5: Constellation Data Structure - [Topic: Dictionary & List of Objects]
# =========================================================================
constellation_db = {
    "big_dipper": [
        MissionStar(-250, -50, "Alkaid", color="#FF8C00"),    
        MissionStar(-190, -45, "Mizar", color="#FFD700"),     
        MissionStar(-140, -40, "Alioth", color="#00FF7F"),    
        MissionStar(-90, -60, "Megrez", color="#00FFFF"),     
        MissionStar(-100, -120, "Phecda", color="#FF69B4"),   
        MissionStar(-20, -110, "Merak", color="#FF4500"),     
        MissionStar(-10, -50, "Dubhe", color="#2A0BD9")       
    ],
    "little_dipper": [
        MissionStar(70, 150, "Yildun", color="#E2E8F0"),      
        MissionStar(110, 130, "Urodelus", color="#FEEBC8"),   
        MissionStar(130, 80, "Anwar", color="#CBD5E0"),       
        MissionStar(180, 70, "Pherkad", color="#FBB6CE"),     
        MissionStar(160, 20, "Kochab", color="#BEE3F8"),      
        MissionStar(110, 30, "Ahfa", color="#C6F6D5")         
    ]
}

polaris_star = MissionStar(40, 190, "Polaris (North Star)", color="#63B3ED", size=22)


# =========================================================================
# SECTION 6: Line Decoration and Drawing Function - [Topic: List Slicing]
# =========================================================================
def draw_lines(star_list, color, close_loop_idx=None):
    line_pen = turtle.Turtle()
    line_pen.hideturtle()
    line_pen.speed(2)
    line_pen.color(color)
    line_pen.pensize(2)
    
    line_pen.penup()
    line_pen.goto(star_list[0].x, star_list[0].y)
    line_pen.pendown()
    
    # Iterating through list slicing [Topic: List Slicing]
    for star in star_list[1:]:
        line_pen.goto(star.x, star.y)
        screen.update()
        time.sleep(0.3)
        
    if close_loop_idx is not None:
        line_pen.goto(star_list[close_loop_idx].x, star_list[close_loop_idx].y)
        screen.update()


# =========================================================================
# SECTION 7: Scaling 5x Distance Using NumPy Vectors - [Topic: NumPy Vector Math]
# =========================================================================
def draw_pointer_arrow():
    update_ui("Scaling 5x Distance from Merak to Dubhe using NumPy Vectors...")
    time.sleep(1)
    
    arrow_pen = turtle.Turtle()
    arrow_pen.hideturtle()
    arrow_pen.speed(1)
    arrow_pen.color("#FF6B6B")  
    arrow_pen.pensize(3)
    
    merak = constellation_db["big_dipper"][5]
    dubhe = constellation_db["big_dipper"][6]
    
    arrow_pen.penup()
    arrow_pen.goto(merak.x, merak.y) 
    arrow_pen.pendown()
    arrow_pen.goto(dubhe.x, dubhe.y) 
    
    # Initializing NumPy Arrays for vector mathematical calculation [Topic: NumPy Arrays]
    p_merak = np.array([merak.x, merak.y])
    p_dubhe = np.array([dubhe.x, dubhe.y])
    
    # Calculating the direction vector from Merak to Dubhe
    direction_vector = p_dubhe - p_merak
    
    arrow_pen.pensize(2)
    for i in range(1, 6):
        update_ui(f"NumPy Vector Step: {i}x ...")
        
        if i == 5:
            # Using Tuple Unpacking [Topic: Tuple Unpacking]
            target_x, target_y = polaris_star.x, polaris_star.y
        else:
            # NumPy broadcasting multiplication and vector addition [Topic: Vector Arithmetic]
            target_point = p_dubhe + (direction_vector * i)
            target_x, target_y = target_point[0], target_point[1]
        
        arrow_pen.goto(target_x, target_y)
        screen.update()
        time.sleep(0.6)


# =========================================================================
# SECTION 8: Matplotlib Constellation Map Generator - [Topic: Matplotlib Scatter Plot]
# =========================================================================
def show_matplotlib_map():
    """Generates a separate 2D statistical star chart after Turtle closes."""
    print("[Matplotlib]: Generating 2D Analytics Plot...")
    
    # Setting dark background theme
    plt.style.use('dark_background')
    fig, ax = plt.subplots(figsize=(7, 5))
    fig.canvas.manager.set_window_title('Matplotlib: 2D Star Chart Data')
    
    # Extracting coordinates for data visualization
    bd_x = [s.x for s in constellation_db["big_dipper"]]
    bd_y = [s.y for s in constellation_db["big_dipper"]]
    
    ld_x = [s.x for s in constellation_db["little_dipper"]]
    ld_y = [s.y for s in constellation_db["little_dipper"]]
    
    # Creating Scatter Plots
    ax.scatter(bd_x, bd_y, color='orange', s=100, label='Big Dipper')
    ax.scatter(ld_x, ld_y, color='cyan', s=60, label='Little Dipper')
    ax.scatter([polaris_star.x], [polaris_star.y], color='white', s=200, marker='*', label='Polaris (North Star)')
    
    ax.set_title("Constellation Coordinate Mapping (Data Analytics View)")
    ax.set_xlabel("X Coordinates")
    ax.set_ylabel("Y Coordinates")
    ax.legend()
    ax.grid(True, linestyle='--', alpha=0.3)
    
    # Set block=True so the window persists on screen
    plt.show(block=True)


# =========================================================================
# SECTION 9: Main Simulator Engine with Exception Control - [Topic: Try-Except-Else]
# =========================================================================
def start_simulator():
    test_star = constellation_db["big_dipper"][0]
    
    try:
        # Running verification check [Topic: Try Block]
        if not isinstance(test_star.x, (int, float)):
            raise InvalidStarDataError("Critical Error: Star coordinates must be numeric!")
            
    except InvalidStarDataError as error_msg:
        # Catching the custom inherited error [Topic: Except Block]
        print(f"[Simulation Interrupted]: {error_msg}")
        update_ui("Data Verification Failed! Process Stopped.")
        
    else:
        # Executes only if no exceptions were raised in the try block [Topic: Else Block]
        print("[System Check]: All star coordinates are valid. Launching Simulation...")
        
        big_dipper = constellation_db["big_dipper"]
        little_dipper = constellation_db["little_dipper"]
        
        # --- Part 1: Drawing the Big Dipper ---
        update_ui("Step 1: Mapping the Colorful Big Dipper Constellation...")
        time.sleep(1)
        for i, star in enumerate(big_dipper):
            star.draw()
            update_ui(f"Drawing Big Dipper: Star {i+1}/7 ({star.name})")
            screen.update()
            time.sleep(0.8)
        draw_lines(big_dipper, "#4FD1C5", close_loop_idx=3)
        time.sleep(1)
        
        # --- Part 2: Finding Polaris using NumPy Vector Calculation ---
        draw_pointer_arrow()
        polaris_star.draw()
        update_ui("SUCCESS! Found Polaris (North Star)! This is TRUE NORTH!")
        screen.update()
        time.sleep(2.5)
        
        # --- Part 3: Drawing the Little Dipper ---
        update_ui("Step 2: Revealing the Little Dipper connected to Polaris...")
        time.sleep(1)
        for i, star in enumerate(little_dipper):
            star.draw()
            update_ui(f"Drawing Little Dipper: Star {i+2}/7 ({star.name})")
            screen.update()
            time.sleep(0.8)
        
        full_little_dipper = [polaris_star] + little_dipper
        draw_lines(full_little_dipper, "#63B3ED", close_loop_idx=3)
        time.sleep(1.5)
        
        # --- Part 4: Twinkling Animation Cycle ---
        update_ui("Look! The stars are twinkling in the night sky!")
        all_stars = big_dipper + [polaris_star] + little_dipper
        for _ in range(4):
            for star in all_stars: star.draw(custom_color="#0B0F19")
            screen.update()
            time.sleep(0.25)
            for star in all_stars: star.draw()
            screen.update()
            time.sleep(0.25)
        
        update_ui("Cosmic Mission Complete! Closing Animation to Launch Matplotlib...")
        screen.update()
        time.sleep(2)
        
        # Safely close the Turtle graphics window
        turtle.bye()


# =========================================================================
# SECTION 10: Execution Control
# =========================================================================
screen.tracer(0)
screen.ontimer(start_simulator, 500)

try:
    screen.mainloop()
except (turtle.Terminator, SystemExit):
    # Catches the termination of Turtle window and seamlessly switches to Matplotlib
    pass

# Launching the separate Matplotlib window safely
show_matplotlib_map()