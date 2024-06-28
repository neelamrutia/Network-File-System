# Network File System (NFS)

## Overview

Welcome to the Network File System (NFS) implementation! This project provides a comprehensive and efficient way to manage files across a distributed network. By utilizing a client-server architecture, the NFS allows multiple clients to interact seamlessly with a shared file system, ensuring consistent and reliable access to data.

## Key Components

1. **Clients**: 
   - **Role**: Clients are the systems or users requesting access to files within the NFS.
   - **Functionality**: They initiate various file-related operations such as reading, writing, deleting, and more, serving as the primary interface for interaction with the NFS.

2. **Naming Server**:
   - **Role**: Acts as a central hub for communication between clients and storage servers.
   - **Functionality**: Provides clients with information about which storage server contains the requested file or folder. It acts as a directory service, ensuring efficient and accurate file access.

3. **Storage Servers**:
   - **Role**: Responsible for the physical storage and retrieval of files and folders.
   - **Functionality**: Manage data persistence and distribution across the network, ensuring files are stored securely and efficiently.

### Core Operations

Clients in the NFS can perform a suite of essential file operations, which include:

- **Writing a File/Folder**: Create and update the content of files and folders within the NFS, ensuring the system remains dynamic and up-to-date.
- **Reading a File**: Retrieve the contents of files stored within the NFS, providing access to necessary information.
- **Deleting a File/Folder**: Remove files and folders from the NFS when they are no longer needed, aiding in efficient space management.
- **Creating a File/Folder**: Generate new files and folders, facilitating the expansion and organization of the file system.
- **Listing All Files and Folders in a Folder**: Retrieve comprehensive listings of files and subfolders within a specified directory, making navigation and management of the file system effortless.
- **Getting Additional Information**: Access supplementary information about specific files, including details such as file size, access rights, timestamps, and other metadata, providing comprehensive insights into the files.


## Project Implementation

## 1. Naming and Storage Servers

### 1.1 Initialization

The initialization process lays the foundation for the NFS. Here are the steps I followed:

1. **Initialize the Naming Server (NM)**:

    - Compiled the Naming Server code using the following command:

    ```sh
    cd nm_server_client
    gcc main.c
    ./a.out
    ```
   - Developed and deployed the Naming Server to serve as the central coordination point.
   - Implemented functionality to manage the directory structure and maintain essential information about file locations.

2. **Initialize Storage Server 1 (SS_1)**:

    - Compiled the Storage Server code using the following command:

    ```sh
    cd storage_1
    gcc ss_1.c
    ./a .out
    ```
   - Developed and deployed Storage Server 1 (SS_1).
   - Implemented functionality for SS_1 to send its details to the Naming Server, including:
     - `IP address` for communication.
     - Port for NM connection.
     - Port for client connection.
     - List of `accessible paths`.

3. **Initialize SS_2 to SS_n**:

    - Compiled the `nth` Storage Server code using the following command:

    ```sh
    cd storage_n
    gcc ss_n.c
    ./a .out
    ```
   - Developed and deployed additional Storage Servers (SS_2 to SS_n).
   - Each server was configured to send its details to the Naming Server following the same procedure as SS_1.

4. **NM Starts Accepting Client Requests**:
   - Configured the Naming Server to start accepting client requests once all Storage Servers were initialized and registered.

### 1.2 On Storage Servers (SS) 

I equipped the Storage Servers with the following functionalities:

1. **Adding New Storage Servers**:
   - Enabled `dynamic addition of new Storage Servers` to the Naming Server at any point during execution.

2. **Commands Issued by NM**: 
   - Implemented the ability for the Naming Server to issue specific commands to Storage Servers:
    - **Create an Empty File/Directory**: Storage Servers can create empty files or directories upon instruction from the NM.
 
    - **Delete a File/Directory**: Storage Servers can delete files or directories as commanded by the NM.

    - **Copy Files/Directories**: Storage Servers can copy files or directories from other Storage Servers with NM providing the relevant IP addresses for data transfer.


3. **Client Interactions**:
     - Implemented the following functionalities for client interactions:
     - **Read a File**: Clients can request Storage Servers to retrieve and send file content.

     - **Write to a File**: Clients can send data to Storage Servers for writing to files.

     - **Get Size and Permissions**: Clients can query Storage Servers to retrieve file information such as size and access permissions.

### 1.3 On Naming Server (NM)

I ensured the Naming Server performs the following key functions:

1. **Storing Storage Server Data**:
   - The NM stores critical information provided by Storage Servers upon connection, acting as the central repository.

2. **Client Task Feedback**:
   - Implemented feedback mechanisms for the NM to provide timely and relevant feedback to clients upon task completion.

## 2. Clients

I implemented the following functionalities for clients:

1. **Directory Mounting**:
   - While the original specification for directory mounting was removed, I provided the option for clients to pass file paths directly to the NM.

2. **File Operations**:

    - usage: 

    ```
    READ <file_path>
    WRITE <file_path>
    DETAILS <file_path>
    ```

   - Clients can initiate file-related operations such as reading, writing, or obtaining information by providing the file path to the NM.
   - The NM identifies the correct Storage Server and facilitates direct communication between clients and the designated Storage Server.
   - Implemented acknowledgment and feedback mechanisms to ensure clients are informed of task status.

   <br>

3. **Creating and Deleting Files and Folders**:

    - usage:

    ```
    DELETE <file_path> or DELETE <'directory_path'+'/'>  
    CREATE <file_path> or CREATE <'directory_path'+'/'>
    ```

   - Clients can request file and folder creation or deletion by providing the respective path and action to the NM.
   - The NM forwards the request to the appropriate Storage Server for execution and provides feedback to the client.

   <br>

4. **Copying Files/Directories Between Storage Servers**:

    - usage: 

    ```
    enter <source_path>
    enter <destination_path>
    ```

   - Clients can request to copy files or directories between Storage Servers by providing source and destination paths.
   - The NM manages the transfer process and provides acknowledgment upon completion.

    <br>

## 3. Other Features

### 3.1 Multiple Clients

I ensured the NFS supports concurrent client access:

- Implemented mechanisms for the NM to handle multiple client requests simultaneously.
- Configured the NM to respond with initial acknowledgments and final acknowledgments from Storage Servers to manage client requests efficiently.

### 3.2 Error Codes

Implemented a set of error codes for robust error handling:

- Defined distinct error codes for various situations, such as file unavailability or write conflicts, to enhance communication between the NFS and clients.

### 3.3 Search in Naming Server

Optimized the search process in the NM:

- Utilized efficient data structures such as Tries and Hashmaps to quickly identify the correct Storage Server for a given request.
- Implemented LRU (Least Recently Used) caching to expedite subsequent requests for the same data.

### 3.4 Redundancy/Replication

Implemented redundancy and replication strategies for data:

- Enabled failure detection for Storage Servers to respond promptly to disruptions.
- Implemented data redundancy by duplicating files and folders in multiple Storage Servers.
- Configured asynchronous duplication for write commands to ensure fault tolerance.

### 3.5 Bookkeeping

Developed a logging mechanism for the NM:

- Implemented logging of every request or acknowledgment received from clients or Storage Servers.
- Included relevant information such as IP addresses and ports used in each communication to enhance traceability and diagnostics.

## Conclusion

This project successfully implements a robust and efficient Network File System (NFS) with comprehensive functionalities for initializing and managing Naming and Storage Servers, supporting multiple clients, handling errors, optimizing search processes, ensuring data redundancy, and maintaining thorough logs for system monitoring and debugging.


## Installation

- to run the project follow the steps below:

1. Clone the repository:
   ```sh
    git clone <repository_url>
    cd <repository_name>
    ```
2. Compile the Naming Server:
   ```sh
   cd nm_server_client
   gcc main.c
   ./a.out
   ```
3. Compile the nth Storage Servers:
    ```sh
    cd storage_n
    gcc ss_n.c
    ./a.out
    ```
4. Run the client Server:
    ```sh
   cd nm_server_client
   gcc client.c
   ./a.out
   ```