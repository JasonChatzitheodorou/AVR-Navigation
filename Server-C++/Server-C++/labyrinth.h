//#include <stdio.h>

#define F_CPU 8000000UL
#define l_size 7
#define q_size 54

/*
int labyrinth[l_size][l_size] = 
                    {   {1, 0, 1, 1, 1}, 
                        {1, 1, 1, 0, 1},
                        {0, 1, 0, 1, 1},
                        {1, 0, 0, 1, 0},
                        {1, 0, 0, 1, 1} };
*/

int labyrinth[l_size][l_size] = 
					{	{1, 1, 1, 1, 1, 1, 1},
						{1, 0, 0, 0, 1, 0, 0},
						{1, 0, 1, 1, 1, 1, 1},
						{0, 0, 1, 0, 0, 0, 1},
						{1, 1, 1, 1, 1, 0, 1},
						{1, 0, 0, 0, 0, 0, 1},
						{1, 1, 1, 0, 1, 1, 1}	};

int parent[l_size * l_size];
int visited[l_size * l_size];
int queue[q_size];

int pos(int x, int y){
    return x * l_size + y;
}

int y_pos(int pos){
    return pos % l_size;
}

int x_pos(int pos){
    return (pos / l_size) % l_size;
}

int check_pos(int x, int y){
    return ((x >= 0 && x < l_size) && (y >= 0 && y < l_size) && !visited[pos(x,y)] && labyrinth[x][y]);
}

void BFS(int pos_start, int pos_end){
    int front = q_size - 1, back = q_size - 1;
    int curr, x, y, temp;

    // push_front
    queue[front--] = pos_start;

    while(front <= back){
        //pop_back
        curr = queue[back--];
        visited[curr] = 1;

        if(curr == pos_end) break;
        
        x = x_pos(curr);
        y = y_pos(curr);

        // Down
        if(check_pos(x+1, y)) {
            temp = pos(x+1, y);
            queue[front--] = temp;
            parent[temp] = curr;
        }

        //Up
        if(check_pos(x-1, y)) {
            temp = pos(x-1, y);
            queue[front--] = temp;
            parent[temp] = curr;
            visited[temp] = 1;
        }
        // Right
        if(check_pos(x, y+1)) {
            temp = pos(x, y+1);
            queue[front--] = temp;
            parent[temp] = curr;
            visited[temp] = 1;
        }

        // Left
        if(check_pos(x, y-1)) {
            temp = pos(x, y-1);
            queue[front--] = temp;
            parent[temp] = curr;
            visited[temp] = 1;
        }
    }
}

// Returns the index of queue on which the directions begin 
int BFS_start(int x_start, int y_start, int x_end, int y_end){
    for(int i = 0; i < l_size * l_size; i++){
        parent[i] = -1;
        visited[i] = 0;
    }

    BFS(pos(x_start, y_start), pos(x_end, y_end));

    int start = q_size - 1;
    int temp = pos(x_end, y_end), p = parent[temp];
    int x, y, x_p, y_p;
    if(visited[temp]){
        while(p != -1){
            x = x_pos(temp);
            y = y_pos(temp);
            x_p = x_pos(p);
            y_p = y_pos(p);
			
			/* 
			 * 0 - Down
			 * 1 - Left
			 * 2 - Up
			 * 3 - Right
			*/
            if(x == x_p + 1) queue[start--] = 0;	
            else if(x == x_p - 1) queue[start--] = 2;
            else if(y == y_p + 1) queue[start--] = 3;
            else if(y == y_p - 1) queue[start--] = 1;

            temp = p;
            p = parent[p];
        }

        start++;
		
		return start;
    }
    else{
        queue[start] = -1;
		//lcd_print_string("Impossible\n");
		_delay_ms(500);
		
		return -1;
    }
}
