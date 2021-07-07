import math

coordinates = {'1': (0,0),
               '2': (1,0),
               '3': (2,0),
               '4': (0,1),
               '5': (1,1),
               '6': (2,1),
               '7': (0,2),
               '8': (1,2),
               '9': (2,2),
               '*': (0,3),
               '0': (1,3),
               '#': (2,3)}

def calculate_distance(key1, key2):
    '''
    Given 2 keys this function calculates the euclidean distance between them using
    the coordinates dictionary.
    
    Arguments:
        key1 [str] : the starting key which signifies the current position
        key2 [str] : the target key which signifies the last position
    
    Returns:
        distance [float] : The euclidean cost of traversing from key1 to key2
    '''
    x1, y1 = coordinates[key1]
    x2, y2 = coordinates[key2]
    distance = math.sqrt((x1-x2)**2 + (y1-y2)**2)
    return distance


def compute_laziest_path_greedy(phone_number:str):
    '''
    Given a phone number without spaces and only keypad specific characters
    this function provided a localy greedy approach to press all keys with two fingers
    
    Arguments:
        phone_number [str] : a string indicating a phone number
    
    Returns:
        result [tuple(float, list[tuple(str,str)])] : The result idicating both the total euclidean cost of pressing 
                                                      all the buttons and their path as list of tuples
    '''
    phone_number_list = [i for i in phone_number]
    left_finger_pos = '*'
    right_finger_pos = '#'
    current_position = (left_finger_pos,
                        right_finger_pos) # left, right
    positions = []
    total_euclidean_distance = 0
    positions.append(current_position)
    for number in phone_number_list:
        left_finger_distance = calculate_distance(current_position[0],
                                                  number)
        right_finger_distance = calculate_distance(current_position[1],
                                                  number)
        if left_finger_distance < right_finger_distance:
            total_euclidean_distance+=left_finger_distance
            left_finger_pos = number
        else:
            total_euclidean_distance+=right_finger_distance
            right_finger_pos = number
        
        current_position = (left_finger_pos,
                            right_finger_pos)
        positions.append(current_position)
            
    return (total_euclidean_distance, positions)

def appendCombinations(arr, n):
    global allPossibleCombinations
    singleCombination = []
    for i in range(0, n):
        singleCombination.append(arr[i])
    allPossibleCombinations.append(singleCombination)

def generateAllFingerCombinationsRecursively(n, arr, i=0):
    if i == n:
        appendCombinations(arr, n)
        return
    arr[i] = 'left'
    generateAllFingerCombinationsRecursively(n, arr, i + 1)
    arr[i] = 'right'
    generateAllFingerCombinationsRecursively(n, arr, i + 1)
    
def generateAllFingerCombinations(n):
    '''
    Generate all possible left or right finger movements for "n" steps
    
    Arguments:
       n [int] : signifies the the number of steps to generate combinations for
    
    Returns:
       allPossibleCombinations [list[list[str]]] : A list of possible finger combinations
    '''
    global allPossibleCombinations
    allPossibleCombinations = []
    placeholder_array = [None] * n
    generateAllFingerCombinationsRecursively(n, placeholder_array)
    return allPossibleCombinations

def get_cost_for_path(phone_number_list, path):
    '''
    Get cost for a path given the order of fingers used
    
    Arguments:
        phone_number_list [list[str]] : The phone Number in question as  list for 
                                   computing total distance
        path [list[str]] : A list of instruction on which finger to use for each step
    
    Returns:
        total_euclidean_distance [float]: the total eculidean cost of the traversed 
                                          path
    '''
    left_finger_pos = '*'
    right_finger_pos = '#'
    current_position = (left_finger_pos,
                        right_finger_pos) # left, right
    
    positions = []
    total_euclidean_distance = 0
    for number, finger in zip(phone_number_list, path):
        if finger=='left':
            finger_distance = calculate_distance(current_position[0],
                                                      number)
            left_finger_pos=number
        else:
            finger_distance = calculate_distance(current_position[1],
                                                      number)
            right_finger_pos=number
            
        current_position = (left_finger_pos,
                            right_finger_pos)
        total_euclidean_distance+=finger_distance
    return total_euclidean_distance


def compute_laziest_best_global_path(phone_number:str):
    '''
    Given a phone number without spaces and only keypad specific characters
    this function provided the optimal approach to press all keys with two fingers
    
    Arguments:
        phone_number [str] : a string indicating a phone number
    
    Returns:
        result [tuple(float, list[tuple(str,str)])] : The result idicating both the total euclidean cost of pressing 
                                                      all the buttons and their path as list of tuples
    '''
    phone_number_list = [i for i in phone_number]
    
    # get all possible path combinations for given phone number lenght
    costPerCombination = {get_cost_for_path(phone_number_list, i):i for i in 
                          generateAllFingerCombinations(len(phone_number_list))}
    bestCombination = costPerCombination[min(costPerCombination)]
    
    
    # initialize agent positions
    left_finger_pos = '*'
    right_finger_pos = '#'
    current_position = (left_finger_pos,
                        right_finger_pos) # left, right
    positions = []
    total_euclidean_distance = 0
    positions.append(current_position)
    
    # choose actions based on best possible combination
    for number, finger in zip(phone_number_list, bestCombination):
        
        if finger == 'left':
            left_finger_pos = number
        else:
            right_finger_pos = number

        current_position = (left_finger_pos,
                            right_finger_pos)
        positions.append(current_position)
    
    # return path with the precomputed best per combination eucliedean cost
    return min(costPerCombination), positions