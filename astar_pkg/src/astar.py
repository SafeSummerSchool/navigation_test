
#clear = lambda: os.system('cls')
#clear()
# -----------
# User Instructions:
#
# Modify the the search function so that it becomes
# an A* search algorithm as defined in the previous
# lectures.
#
# Your function should return the expanded grid
# which shows, for each element, the count when
# it was expanded or -1 if the element was never expanded.
# In case the obstacles prevent reaching the goal,
# the function should return "Fail"
#
# You do not need to modify the heuristic.
# ----------

grid = [[0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0]]

#heuristic = [[9, 8, 7, 6, 5, 4],
#            [8, 7, 6, 5, 4, 3],
#            [7, 6, 5, 4, 3, 2],
#            [6, 5, 4, 3, 2, 1],
#            [5, 4, 3, 2, 1, 0]]

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]

heuristic = [[0 for row in range(len(grid[0]))] for col in range(len(grid))]
for nn in range(len(grid[0])):
    for n in range(len(grid)):
        heuristic[n][nn] = abs(goal[0]-n)+abs(goal[1]-nn)



delta = [[-1, 0 ], # go up
         [ 0, -1], # go left
         [ 1, 0 ], # go down
         [ 0, 1 ]] # go right

delta_name = ['^', '<', 'v', '>']

cost = 1

# ----------------------------------------
# modify code below
# ----------------------------------------

closed = [[0 for row in range(len(grid[0]))] for col in range(len(grid))]
closed[init[0]][init[1]] = 1
expand = [[-1 for row in range(len(grid[0]))] for col in range(len(grid))]
    #action = [[-1 for row in range(len(grid[0]))] for col in range(len(grid))]



def search():
    x = init[0]
    y = init[1]
    g = 0

    open = [[g, x, y]]
    found = False  # flag that is set when search is complete
    resign = False # flag set if we can't find expand
    count = 0
    while not found and not resign:
        if len(open) == 0:
            resign = True
            #return "Fail"
            return "Fail"
        else:
            open_P_Heur = [];
            for i in range(len(open)):
                open_P_Heur.append([open[i][0]+heuristic[open[i][1]][open[i][2]],i])   #,open[i][1],open[i][2]])
            open_P_Heur.sort()
            #print open_P_Heur
            next = open.pop(open_P_Heur[0][1])
            x = next[1]
            y = next[2]
            g = next[0]
            expand[x][y] = count
            count += 1
        
            if x == goal[0] and y == goal[1]:
                found = True
            else:
                for i in range(len(delta)):
                    x2 = x + delta[i][0]
                    y2 = y + delta[i][1]
                    if x2 >= 0 and x2 < len(grid) and y2 >=0 and y2 < len(grid[0]):
                        if closed[x2][y2] == 0 and grid[x2][y2] == 0:
                            g2 = g + cost
                            open.append([g2, x2, y2])
                            closed[x2][y2] = 1   
    return expand
expand = search()
roadMap = []
tmpPosition = []
reachedGoal = False
cPosition = init
while not reachedGoal:
    x = cPosition[0]
    y = cPosition[1]
    
    maxVal = 0
    addMove = False
    for i in range(len(delta)):
        x2 = x + delta[i][0]
        y2 = y + delta[i][1]
        
        if (x2 == goal[0]) and (y2 == goal[1]):
            reachedGoal = True
        
        if x2 >= 0 and x2 < len(grid) and y2 >=0 and y2 < len(grid[0]):
            if maxVal<expand[x2][y2]:
                maxVal = expand[x2][y2]
                #print x2, y2, maxVal
                cPosition = [x2, y2]
                #addMove = True
                
        #if addMove:
    roadMap.append(cPosition)
                    
for i in range(len(heuristic)):
    print heuristic[i]        
for i in range(len(expand)):
    print expand[i]
#    for i in range(len(closed)):
    #    print closed[i]
for i in range(len(roadMap)):
    print roadMap[i]
#return expand #Leave this line for grading purposes!

#search()
