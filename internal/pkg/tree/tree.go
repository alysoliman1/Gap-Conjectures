package tree

type Node struct {
	ID    string
	Level int
	Xi    float32
	Left  *Node
	Right *Node
}
