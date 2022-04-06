#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

#define BIRTHDAYS_NUM 7

/* Create birthday struct */
struct birthday 
{
    int day;
    int month;
    int year;
    struct list_head list;
};

/* Intializeed the head node of the linked list. */
LIST_HEAD(birthday_list);

/* Load_birthdays function. */
int load_birthdays(void)
{
    /* Allocate memory for 7 birthday struct elements in the linked list. */
    struct birthday *person;
    int i = 0;
    for(i = 0; i < BIRTHDAYS_NUM; i++){
        /* kmalloc is the method of allocating the memory in the kernel module. */
        person = kmalloc(sizeof(*person), GFP_KERNEL);
        person->day = 3 + i;
        person->month = 6 + i;
        person->year = 1997 + i;
        /* Intializeed the head node of the linked list. */
        INIT_LIST_HEAD(&person->list);
	/* Add the new node to the linked list. */
        list_add_tail(&person->list, &birthday_list);
    }

    /*  Traverse linked list and print the data of each node on the kernel log. */
    list_for_each_entry(person, &birthday_list, list) {
        printk(KERN_INFO "Load Birthday: %d/%d/%d\n", person->month, person->day, person->year);
    }
    return 0;
}

/* When kernel module loaded, it will run this fuction. */
/* Execute load_birthdays function. */
int simple_init(void) 
{
  printk(KERN_INFO "Loading Kernel Module\n");
  load_birthdays();
  return 0;
}

/* Remove_birthdays function. */
int remove_birthdays(void) {
  /* Traverse the linked list and delete its nodes. */
  struct birthday *ptr, *next;
  /* Remove the data of linked list and print the data of each node on the kernel log. */
  list_for_each_entry_safe(ptr, next, &birthday_list, list) {
      printk(KERN_INFO "Remove Birthday: %d/%d/%d\n", ptr->month, ptr->day, ptr->year);
      list_del(&ptr->list);
      kfree(ptr);
  }
  return 0;
}

/* When kernel module removed, it will run this fuction. */
/* Execute remove_birthdays function. */
void simple_exit(void) {
    printk(KERN_INFO "Removing Kernel Module\n");
    remove_birthdays();
}

module_init(simple_init);
module_exit(simple_exit);
