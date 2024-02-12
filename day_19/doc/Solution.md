Applied Concurrency Idea:
	- Task parallelism
	- Have a thread for each workflow or for a certain amount of workflows
	- Each thread (WT) has an input Queue for the next data to sort
	- Let the main thread (MT) transfer the input to the workers
	
Data Model:
	- Input -> Workflow, MachinePart
		○ Workflow: WorkflowKey : string, list<Rule>
			- Rule: orderId : unsigned, dataId : unsigned, operation : enum, value : unsigned, destination: WorkflowKey
		○ Rule: struct{x, m, a, s}, sum()
		
Component Model:
    - FileHandler - Parse File String by String via Iterator
	- Parser -> generates Workflow, Data via String
	- MT inializes WT with Workflows (equal distribution)
	- WT iterates through Rules und matches WT with Parts
	- WT owns Workflows and Input Queue (atomic or mutex)
	- Final: WT fill accepted/rejected Parts into SumList (atomic oder mutex)
	- MT accumulates Parts via sum()


Time Comparison measurement:
Single threaded vs multi-threaded
