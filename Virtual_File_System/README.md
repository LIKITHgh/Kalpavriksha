# **Virtual File System (VFS) – Assignment & Explanation**

---

## **Problem Statement**

You are tasked with building a **Virtual File System (VFS)** that simulates an **in-memory file storage environment**.  
The system should allow users to **create, read, write, and manage files and directories** using **circular and doubly linked lists** as the core data structures.  
The purpose is to simulate how an actual file system manages memory, directory structures, and disk block allocations.

---

## **Core Initialization and Design Overview**

At program startup, the virtual disk and root directory must be initialized.

- The **virtual disk** is represented as a **two-dimensional byte array**.  
- The **free blocks** are organized in a **doubly linked list** for easy allocation and deallocation.  
- The **root directory '/'** is created during initialization and serves as the starting point of the filesystem hierarchy.

---

## **Free Block Management**

The virtual disk is divided into **fixed-size blocks** (e.g., 512 bytes).  
Each block represents a unit of storage, and all unallocated blocks are maintained in a **doubly linked list** of `FreeBlock` nodes.

At system startup, **all blocks are free** and inserted into the free block list.

Each `FreeBlock` node contains:
- An **index** (indicating its position in the disk array)
- `next` and `prev` pointers

### **Block Handling Rules**
- **When a file is written:**  
  Required number of blocks are removed from the **head** of the free list.

- **When a file is deleted:**  
  Its blocks are reinserted at the **tail** of the free list.

- **When the disk is full:**  
  Write operations must fail with an appropriate message.

---

## **Directory and File Structure**

Each directory and file is represented by a **FileNode** structure.

### **Directory Structure**
- Maintains a **circular linked list** of its child entries
- Child entries may include **files** or **subdirectories**
- Directories **do NOT consume disk blocks**  
  They exist purely in memory as linked lists.

### **File Structure**
- Uses `blockPointers[]` to store references to disk blocks containing file data
- Files consume disk space based on:
  ```
  contentSize / BLOCK_SIZE
  ```

---

## **Current Working Directory (CWD)**

The `cwd` pointer indicates where the user currently is within the filesystem.

### **Behavior**
- On startup → `cwd = '/'`
- `mkdir folder1` → cwd remains `'/'`
- `cd folder1` → cwd becomes `'/folder1'`
- `create notes.txt` → file created under `'/folder1'`, cwd remains unchanged
- `cd ..` → cwd moves to the parent directory

---

## **Operation Explanations**

### **mkdir**
Creates a new directory under the current directory.  
Inserted into the **circular linked list**.

---

### **create**
Creates a new file node under the current directory.  
Files start with **zero allocated blocks**.

---

### **write**
- Allocates one or more blocks from the **free list**
- Writes content to the virtual disk
- Updates the file’s `blockPointers[]`

---

### **read**
Reads file content by traversing `blockPointers[]` and printing stored data from the virtual disk.

---

### **delete**
Deletes a file by:
- Removing it from the directory’s circular linked list  
- Returning its blocks to the **free list**

---

### **rmdir**
Removes a directory **only if it is empty**.  
Fails otherwise.

---

### **ls**
Lists all files and directories under the current directory  
by iterating over the circular linked list.

---

### **cd**
Changes the current working directory by updating the `cwd` pointer.

---

### **pwd**
Displays the full directory path by walking parent pointers up to root.

---

### **df**
Displays disk usage:  
- Total blocks  
- Used blocks  
- Free blocks  

---

### **exit**
- Frees all allocated memory  
- Clears linked lists  
- Terminates the program

---

