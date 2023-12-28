

Our primary testing object is a binary tree. We will store a tree in a dynamo table.
Each node has a unique id, pointer to left and right children, and the level the node is at.


| Field Name | Description |
| ---------- | ----------- |
| ID | Unique identifier |
| Xi | Xi value associated with tree the node is in |
| Level | Level the node is at in the tree |
| Left | Identifier to left node (if it exists) |
| Right | Identifier to right node (if it exists) |


