package main

import (
	"encoding/csv"
	"errors"
	"fmt"
	"io"
	"log"
	"os"
	"strconv"
)

var (
	start  = Node{"start", 0, 0, 0, 0, 0, 0}
	finish = Node{"finish", 0, 0, 0, 0, 0, 0}
)

type Node struct {
	// Name - название вершины
	Name string
	// Weight - вес вершины
	Weight int
	// Es - earliest start
	ES int
	// EF - earliest finish
	EF int
	// LS - latest start
	LS int
	// LF - latest finish
	LF int
	// AF - activity float
	AF int
}

type Graph struct {
	nodes     map[string]Node
	adjacency map[string][]string
}

func NewGraph() Graph {
	return Graph{
		nodes:     make(map[string]Node),
		adjacency: make(map[string][]string),
	}
}

func (g *Graph) addNode(newNode Node) {
	g.adjacency[newNode.Name] = []string{}
	g.nodes[newNode.Name] = newNode
}

func (g *Graph) addEdge(from string, to string) {
	g.adjacency[from] = append(g.adjacency[from], to)
}

func (g *Graph) createVisited() map[string]bool {
	visited := make(map[string]bool, len(g.adjacency))
	for key := range g.adjacency {
		visited[key] = false
	}
	return visited
}

func (g *Graph) findPrevNodes(nodeName string) []string {
	var ret []string

	for node, deps := range g.adjacency {

		for _, dep := range deps {
			if dep == nodeName {
				ret = append(ret, node)
			}
		}
	}

	return ret
}

func (g *Graph) findMaxEF() int {
	max := 0
	for _, node := range g.nodes {
		if node.EF > max {
			max = node.EF
		}
	}

	return max
}

func (g *Graph) backward(endingNode string) {
	visited := g.createVisited()
	maxEF := g.findMaxEF()
	g.backwardRecursive(endingNode, visited, maxEF)
}

func (g *Graph) backwardRecursive(curNodeName string, visited map[string]bool, prevLS int) {
	curNode := g.nodes[curNodeName]

	if visited[curNodeName] == true {
		if curNode.LF > prevLS {
			curNode.LF = prevLS
			curNode.LS = curNode.LF - curNode.Weight
			g.nodes[curNodeName] = curNode
		} else {
			return
		}
	} else {
		visited[curNodeName] = true

		curNode.LF = prevLS
		curNode.LS = curNode.LF - curNode.Weight
		g.nodes[curNodeName] = curNode
	}

	prevNodes := g.findPrevNodes(curNodeName)
	for _, node := range prevNodes {
		g.backwardRecursive(node, visited, curNode.LS)
	}
}

func (g *Graph) forward(startingNode string) {
	visited := g.createVisited()
	g.forwardRecursive(startingNode, visited, 0)
}

func (g *Graph) forwardRecursive(curNodeName string, visited map[string]bool, prevEF int) {
	curNode := g.nodes[curNodeName]

	if visited[curNodeName] == true {
		// если у предыдущей ноды время конца > времени начала текущей ноды, то перезаписываем и пересчитываем с новыми значениями
		if prevEF > curNode.ES {
			curNode.ES = prevEF
			curNode.EF = curNode.ES + curNode.Weight
			g.nodes[curNodeName] = curNode
		} else {
			return
		}
	} else {
		visited[curNodeName] = true

		curNode.ES = prevEF
		curNode.EF = curNode.ES + curNode.Weight
		g.nodes[curNodeName] = curNode
	}

	for _, node := range g.adjacency[curNodeName] {
		g.forwardRecursive(node, visited, curNode.EF)
	}
}

func (g *Graph) countActivityFloat() {
	for nodeName, node := range g.nodes {
		node.AF = node.LS - node.ES
		g.nodes[nodeName] = node
	}
}

func (g *Graph) CriticalPath() {
	g.forward(start.Name)
	g.backward(finish.Name)
	g.countActivityFloat()
	for nodeName, node := range g.nodes {
		if node.AF == 0 && node.Name != start.Name && node.Name != finish.Name {
			fmt.Println(nodeName, node.Weight)
		}
	}
}

func (g *Graph) Print() {
	for i := range g.nodes {
		fmt.Printf("Nodes[%v] : %+v\n", i, g.nodes[i])
	}

	for i := range g.adjacency {
		fmt.Printf("Edges[%v] : %v\n", i, g.adjacency[i])
	}
}

func (g *Graph) ReadeNodes(filename string) {
	file, err := os.Open(filename)
	if err != nil {
		log.Fatalf("can't open nodes file: %v", err)
	}

	reader := csv.NewReader(file)
	// разделитель
	reader.Comma = ','

	for {
		line, err := reader.Read()
		if errors.Is(err, io.EOF) {
			break
		}

		weight, err := strconv.Atoi(line[1])
		if err != nil {
			log.Fatalf("weight mast be int, but got: %v", line[1])
		}

		g.addNode(Node{line[0], weight, 0, 0, 0, 0, 0})
	}
}

func (g *Graph) startAndFinish() {
	for key, value := range g.adjacency {
		if len(value) == 0 {
			g.addEdge(key, finish.Name)
		}
	}

	g.addNode(start)
	g.addNode(finish)

	for node, _ := range g.nodes {
		edges := g.findPrevNodes(node)
		if len(edges) == 0 && node != start.Name {
			g.addEdge(start.Name, node)
		}
	}
}

func (g *Graph) ReadEdges(filename string) {
	file, err := os.Open(filename)
	if err != nil {
		log.Fatalf("can't open edges file: %v", err)
	}

	reader := csv.NewReader(file)
	// разделитель
	reader.Comma = ','

	for {
		line, err := reader.Read()
		if errors.Is(err, io.EOF) {
			break
		}

		for i := 1; i < len(line); i++ {
			g.addEdge(line[0], line[i])
		}
	}

	g.startAndFinish()
}

func main() {
	graph := NewGraph()

	graph.ReadeNodes("nodes.csv")
	graph.ReadEdges("edges.csv")

	graph.CriticalPath()
}
