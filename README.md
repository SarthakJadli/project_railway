# 🚄 Railway Recommendation System

A Python project for railway route recommendation, shortest path finding, and ticket management using graph and queue algorithms.

---

## 🔍 Features

- Find shortest routes using **Dijkstra’s** and **A\*** algorithms
- Ticket **booking and cancellation** using **queue** methods
- Show train **routes on a map** 
- Traverse the railway network using **BFS** and **DFS**

---

## 🧠 Algorithms Used

| Algorithm   | Purpose                                |
|------------|----------------------------------------|
| Dijkstra   | Find shortest path (based on weights)  |
| A\* Search | Heuristic route optimization           |
| BFS        | Level-wise route traversal             |
| DFS        | Deep traversal for all connections     |
| Queue      | Booking and cancellation management    |

---
 Example Use Case
User input: Source = Delhi, Destination = Kolkata

Dijkstra Output: Delhi ➡️ Kanpur ➡️ Varanasi ➡️ Kolkata

Booking Status: Ticket added to queue (Position 3)

Cancellation Status: Removed from queue

Route Map: A visual graph showing the railway path
