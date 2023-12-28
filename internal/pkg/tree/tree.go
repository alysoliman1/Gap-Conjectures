package tree

type Node struct {
	ID    string
	Left  *Node
	Right *Node
}

type Tree struct {
	Root       *Node
	LevelCount []int
}

func NewTree(depth int) *Tree {
	return &Tree{
		Root:       new(Node),
		LevelCount: make([]int, depth),
	}
}

func (t *Tree) NumPaths(depth int) int {
	return 0
}
