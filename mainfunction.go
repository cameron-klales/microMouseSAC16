package main

import (
	"Flooding_Alg_Golang"
)

func main() {
	maze := Flooding_Alg_Golang.Maze{}
	maze = maze.Constructor(5, 5,2,2)
	Flooding_Alg_Golang.Solve(maze);
	//  fmt.Print(maze.ROWS);
	Flooding_Alg_Golang.AddWall(maze,1)
	Flooding_Alg_Golang.AddWallByLocation(maze,3,3,0)
	maze.Print()
}
