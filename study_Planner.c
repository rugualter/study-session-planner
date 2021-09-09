#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
 
#define MAXSTR 128 // definition of maximum size for the lines that lemon the file
 
// Course data structure for formative activities
typedef struct struct_data_af{
    int uc_number;
    char *uc_name;
} S_data_af;
 
 // struracy Data for curricular unudades
typedef struct struct_data_uc{
    int uc_number;
    char *uc_name;
} S_data_uc;
 
 // List of data from curricular units
typedef struct List_uc_data{
    S_data_uc uc_data;
    struct List_uc_data *next;
} L_uc_data;

// List of formative activities data
typedef struct List_af_data{
    int number_uc;
    int start;
    int end;
    int done;
    int n_sessions;
    char *af_name;
    struct List_af_data *next;
} L_af_data;

// List of data from curriculatres units with notes
typedef struct List_grade_data{
    char *ucname;
    int ucnumber;
    double percent;
    int grade;
    struct List_grade_data *next;
} L_grade_data;
 
 // list that keeps the value k to be collected from the file (I took the wave of the structures =))
 typedef struct List_k_data{
    int k;
    struct List_k_data *next;
} L_k_data;

// List of data with the information of the days of sessions to be performed
typedef struct List_af_dates_data{
    int ucnumb;
    char *nam_af;
    int *days;
    int i; 	// is transported to know how many days should I print
    struct List_af_dates_data *next;
} L_af_dates_data;
 
// funcao that adds given to the list of curricular units
L_uc_data* UC_add(L_uc_data *list, int number, const char *name)
{
    L_uc_data *new = malloc(sizeof(L_uc_data));
    
	if (new != NULL) {
       
	   new->uc_data.uc_number = number;
       new->uc_data.uc_name =(char*)malloc(strlen(name)+1);
       strcpy(new->uc_data.uc_name,name);
       new->next = list;
       return new;
    }
	
    return list;
}
 
// Function adding data to the list of training activities
L_af_data* AF_add(L_af_data *list1, int num, int begin, int finish, int solved, int sessions, const char *name_af)
{
    L_af_data *new1 = malloc(sizeof(L_af_data));
    
	if (new1 != NULL) {
		
        new1->number_uc = num;
        new1->start = begin;
        new1->end = finish;
        new1->done = solved;
        new1->n_sessions = sessions;
        new1->af_name =(char*)malloc(strlen(name_af)+1);
        strcpy(new1->af_name,name_af);
        new1->next = list1;
        return new1;
    }
	
    return list1;
}

// Function Keeping K
L_k_data* K_add(L_k_data *list3, int Ks)
{
    L_k_data *new5 = malloc(sizeof(L_k_data));
    
	if (new5 != NULL) {
		
        new5->k = Ks;
        new5->next = list3;
        return new5;
    }
	
    return list3;
}

// funcao which adds given to the list of curricular units with the notes
L_grade_data* GRADE_add(L_grade_data *list2, const char *nameuc, int numuc, double percentage, int note)
{
    L_grade_data *new2 = malloc(sizeof(L_grade_data));
    
	if (new2 != NULL) {
       
	   new2->ucname =(char*)malloc(MAXSTR);
       strcpy(new2->ucname,nameuc);
       new2->ucnumber = numuc;
       new2->percent = percentage;
       new2->grade = note;
       new2->next = list2;
       return new2;
    }
	
    return list2;
}

// Function adding data to the list of days of training sessions of training
L_af_dates_data* AFDATES_add(L_af_dates_data *list, int numberuc, const char *af_nam,  int *dates, int i)
{
    L_af_dates_data *new = malloc(sizeof(L_af_dates_data));
    
	if (new != NULL) {
        
		new->ucnumb = numberuc;
        new->nam_af =(char*)malloc(strlen(af_nam)+1);
        strcpy(new->nam_af,af_nam);
        new->days = (int *)malloc(((i-1)+1)*sizeof(int));
        memcpy(new->days, dates, ((i-1)+1)*sizeof(int));
        new->i = i;
        new->next = list;
        return new;
    }
	
    return list;
}
 
// funcao that releases the memory of the curricular units list
void UC_free(L_uc_data *list)
{
    while (list) {
        
		L_uc_data *aux = list->next;
        free(list->uc_data.uc_name);
        free(list);
        list = aux;
    }
}

// Funcao releasing the memory of the list of training activities
void AF_free(L_af_data *list1)
{
    while (list1) {
		
        L_af_data *aux1 = list1->next;
        free(list1->af_name);
        free(list1);
        list1 = aux1;
    }
}

// Funcao releasing the memory of the list of curricular units with notes
void GRADE_free(L_grade_data *list2)
{
    while (list2) {
		
        L_grade_data *aux2 = list2->next;
        free(list2->ucname);
        free(list2);
        list2 = aux2;
    }
}

// Funcao releasing the memory of the list of curricular units with notes
void AFDATES_free(L_af_dates_data *list)
{
    while (list) {
		
        L_af_dates_data *aux = list->next;
        free(list->nam_af);
		free(list->days);
        free(list);
        list = aux;
    }
}

// Function that releases Kemor of K
void K_free(L_k_data *list3)
{
    while (list3) {
		
        L_k_data *aux3 = list3->next;
        free(list3);
        list3 = aux3;
    }
}
 
// funcao that reads the first part of the file (descriptive of the curricular units)
L_uc_data * UC_read(int *checkline,FILE *f)
{
    char line[MAXSTR];
    L_uc_data *the_list = NULL;

    while (fgets(line, MAXSTR, f)) { // Getting the lines
		
        int number;
        char name[MAXSTR];

		if(line[0]=='\n'){ // verificaçao if the file has empty line
			
			checkline[0]=0;
			return the_list;
			}
			
        if (2 == sscanf(line, "%d - %[^\n]", &number, name)) {// le the lines

            the_list = UC_add(the_list, number, name); // Admiona a list
        }
    }
	
    return the_list;
}

// Funcao Le the second part of the file (description of formative activities)
L_af_data * AF_read(FILE *f)
{
    char line1[MAXSTR];
    L_af_data *the_list1 = NULL;

    while (fgets(line1, MAXSTR, f)&&line1[0]!='\n') {
		
        int num, begin, finish, solved, sessions;
        char name_af[MAXSTR];

        if (6 == sscanf(line1, "%d - %d - %d - %d - %d - %[^\n]", &num, &begin, &finish, &solved, &sessions, name_af)) {

            the_list1 = AF_add(the_list1, num, begin, finish, solved, sessions, name_af);
        }
    }
	
    return the_list1;
}

// Function Lei The third part of the file (get the value of K)
L_k_data * K_read(FILE *f)
{
    char line3[MAXSTR];
    L_k_data *k_list = NULL;

    while (fgets(line3, MAXSTR, f)) {
		
        int number;

        if (1 == sscanf(line3, "%d", &number)) {

            k_list = K_add(k_list, number);
        }
    }
	
    return k_list;
}
 
// Function that shows the total characters, activities, held etc. (Questao 1 and 2)
void UC_show(L_uc_data *list, L_af_data *list1,int *checkline, int k, FILE *fout)
{
    L_af_data *temp = list1; // Guard where the top of the list is
    int total_char=0;
    int total_activities=0;
    int uc_activities=0; 
    int done_uc_activities=0;
    
	// Lists are checked step by step goes through the UC list and for each UC runs the list of activities to get the data
    while (list) {

        int n_char=strlen(list->uc_data.uc_name); // Get the UC characters
        total_char=total_char+n_char; // adds to total

    while (list1) {
 
        if(list->uc_data.uc_number==list1->number_uc){ // verifies that it is in the UC corresponds to the list of activities
			
            ++uc_activities; // ruling activities
        }
		
        if(list->uc_data.uc_number==list1->number_uc && list1->done==1){ // verifies that it is in the UC corresponded to the list of activities and if the activity is performed
			
            ++done_uc_activities; // increments activities carried out
        }

        list1 = list1->next;
    }

        list1=temp; // Repoo the pointer to the top of the list
        list = list->next;
    }
    
    while (list1) { // tells the total activities is separated because the above distraction makes reset to the list soon would increase too much
		
        ++total_activities;
         list1 = list1->next;
    }
	
	// This section prints the results according to the criteria
	if(checkline[0]==1){ // When there is only UC information
		
		fprintf(fout, "%d\n", total_char);
	}
	
	if(checkline[0]==0 && k<=0){ // When there is information for formative activities or the K is 0
		
		fprintf(fout, "%d %d %d %d\n", total_char, total_activities, uc_activities, done_uc_activities);
	}
	
	if(checkline[0]==0 && k>0){ // When K is greater than 0
		
		fprintf(fout, "%d %d %d %d", total_char, total_activities, uc_activities, done_uc_activities);
	}
    
}
 
// Function that calculates the notes and places the data of each UC in the list of curricular units with note
L_grade_data * GRADE_read(L_uc_data *list, L_af_data *list1, FILE *fout)
{
    L_grade_data *the_list2 = NULL;
    L_grade_data *temp2=the_list2; // Guard the top of the list
    L_af_data *temp = list1;
	
    
    int done_sessions=0;
    int total_sessions=0;
    double perc=0;
    int grading=0;
    char nameuc[MAXSTR]; 
    int numuc, note;
    
    while (list) {

		while (list1) {
        
			if(list->uc_data.uc_number==list1->number_uc){
 
				total_sessions=total_sessions+list1->n_sessions; // Calculate the total of sessions
			}
		
			if(list->uc_data.uc_number==list1->number_uc && list1->done==1){
			
                done_sessions=done_sessions+list1->n_sessions; // Calculates the total session performed
            }

			list1 = list1->next;
		}
		
        if(total_sessions>0){
			
        perc=((double)done_sessions/total_sessions)*100; // Calculate the percentage
        }

		// SECCAO that calculates the already rounded note
        if(perc<=100&&perc>=25){
			
            grading=round(((perc-25)/75)*10)+10; 
        }
		
        if(perc<=25&&perc>0){
			
            grading=round(perc/25)*10;
        }

        total_sessions=0;
        done_sessions=0;
        strcpy(nameuc,list->uc_data.uc_name);
        numuc=list->uc_data.uc_number;
        note=grading;
		list1=temp; // puts the pointer again at the top of the list
        the_list2 = GRADE_add(the_list2, nameuc, numuc, perc, note); // adds the elements the list of curricular units with note
        perc=0;
        grading=0;
        list = list->next;
    }
	
    return the_list2;
}

// Funcao Exchanging Strings Elements
void change_string(char *str1, char *str2, size_t size)
{
    char *temp = NULL;
    temp = (char*)malloc(MAXSTR);
	
	// Exchange the elements
    memcpy(temp, str1, size);
    memcpy(str1, str2, size);
    memcpy(str2, temp, size);
	
	free(temp); // free to temporary memory
}

// Funcao reigning the list of curricular units with note by higher note for the lowest and then by UC number lower for higher
void sortlist(L_grade_data *list2)
{
    L_grade_data *new3=list2;
	
	// verify that the percentage is lower if yes change of position
    while (new3 != NULL){
		
		L_grade_data * new4 = new3->next;
		
		while (new4 != NULL){
			
			if (new3->percent < new4->percent){ // if percentage less exchange the contents of the two UC
				
				double temp_percent = new3->percent; 
				new3->percent = new4->percent;
				new4->percent = temp_percent;
				change_string(new3->ucname, new4->ucname, MAXSTR);
				int temp_ucnumber = new3->ucnumber; 
				new3->ucnumber = new4->ucnumber;
				new4->ucnumber = temp_ucnumber;
				int temp_grade = new3->grade; 
				new3->grade = new4->grade;
				new4->grade = temp_grade;
			}
			
			if (new3->percent == new4->percent && new3->ucnumber > new4->ucnumber){// if perenture is equal to exchange the contents of the two UC
            
				double temp_percent = new3->percent; 
				new3->percent = new4->percent;
				new4->percent = temp_percent;
				change_string(new3->ucname, new4->ucname, MAXSTR);
				int temp_ucnumber = new3->ucnumber; 
				new3->ucnumber = new4->ucnumber;
				new4->ucnumber = temp_ucnumber;
				int temp_grade = new3->grade; 
				new3->grade = new4->grade;
				new4->grade = temp_grade;
			}
			
			new4 = new4->next;
		}

		new3 = new3->next;
	}  
}

// Function that makes a hideous printh from the list of curricular units with Note Server for Questao 3
void GRADE_show(L_grade_data *list2, FILE *fout)
{
    while (list2) {

        fprintf(fout, "%s: UC%d %.1lf%% Nota prevista: %d valores\n", list2->ucname, list2->ucnumber, list2->percent, list2->grade);
        list2 = list2->next;
    }
}

// duncao that adds the days of formative activities and the respective activity to be realized to the list of days of formative activities
L_af_dates_data * GET_af(L_grade_data *list2, L_af_data *list1, L_uc_data *list, int k, int *calend, FILE *fout)
{
    L_af_dates_data *the_list = NULL;
	L_af_data *temp = list1;
    L_grade_data *temp2 = list2;
    
    int numruc=0;
    char *namaf; 
    int *datedays;
    int j;
    int i=0;
    int w;
    int day;
    int count=1;
    int session_control;
	int max;
	int lowestgrade;
    int calend_control=0;
    int lowestsession=1000; // a high value was initiated to make later verification
    char *aflowestsession=(char*)malloc(MAXSTR);

    for(w=0; w<31;w++){ // Get the calendar value to verify if it is already filled

        calend_control=calend_control+calend[w];
    }

	while(1){ // keep the loop until the criteria defined within the loop return something

	count=1;
	lowestgrade=0;

		// find the number of the UC with smaller note and the K of greater note
		while(list2){
			
			if(count==k){ // When we are in the UC with lower note
				lowestgrade=list2->ucnumber; // we keep the UC number
			}
			
			list2 = list2->next;
			++count;
		}

		list2=temp2; // We put the new arranger at the top of the list
		count=1; // reset to the accountant
	
		// find the formative activity with smaller number of sessions
		while (list2) {
			
			if(list2->ucnumber==lowestgrade){// check if it is the UC of smaller note
				
				while (list1) {

					if(list2->ucnumber==list1->number_uc && list1->done==0){ // verify whether it is the same UC and verify if it does not have the activity performed
						
						if(lowestsession>list1->n_sessions){ // save data whenever smaller
							
							lowestsession=list1->n_sessions;
							strcpy(aflowestsession,list1->af_name);
						}
					}
					
					list1 = list1->next;
				}
			}
      
			list2 = list2->next;
		}

		list2=temp2;// Repore pointers for the beginning of the lists
		list1=temp;
    
	// SECCAO that will add the values ​​to the list
		while (list2) {
  
			if(list2->ucnumber==lowestgrade){
        
				if(list2->ucnumber==20){ // Loop output criteria if the UC is already with Maxima Note Sai
					
					return the_list;
				}
				
				while(list1){
					
                	if(list2->ucnumber==list1->number_uc && list1->done==1 && strcmp(aflowestsession, list1->af_name) == 0){ // Said criteria when the formative activity is already performed
						return the_list;
					}
					
					if(list2->ucnumber==list1->number_uc && list1->done==0 && strcmp(aflowestsession, list1->af_name) == 0){ // verify that it is a formative activity not performed and is the least session
						
						numruc=list1->number_uc;
						namaf=(char*)malloc(strlen(list1->af_name+1));
						strcpy(namaf,list1->af_name);
						session_control=list1->n_sessions;// parts the control of sessions to subsequently marar the activity as carried out
						
						// SECCAO that checks how many days are available to carry out the sessions to later use in the exit criterion
						if(list1->start==0){
								day=list1->end-list1->start;
						}
                    
						if(list1->start>0){
							day=list1->end-list1->start+1;
						}
						
						datedays=(int *)malloc(sizeof(int));
						int check=0; // Value to be used with Variavavel Days Said Paracriterio
						
						for(j=list1->start; j<=list1->end; j++){ // being selected Formative Activity Let's go through the days and allocalos if possible
							
							if(calend_control==0){ // If there are no vacancies in the calendar exists
								return the_list;
							}
							
							if(calend[j]>0 && session_control>0){ // if there is a place in the calendar and sessions to put then we schedule

								calend[j]=calend[j]-1; // A slot is removed in the calendar
								
								if(i>0){ // and allocated new space always for the days whenever there is a new day for the days string of activity
									
									datedays=realloc(datedays,(i+1)*sizeof(int));
								}
								
								datedays[i]=j; // is scheduled the day
								++i;
								session_control=session_control-1;
								
								if(session_control==0){ // When sessions come to an end then the activity is complete and updated as complete
									
									list1->done=1;
								}
							}
							
							if(calend[j]==0){ //If there is no slot in the calendar we increase the check for Udar subsequently as an output criteria
								
							check=check+1;
							}
						}
						
						if(day-check==0){ //Leave criteria means that if there is no calendar slot for any session of the activity gets out of the loop
							
							return the_list;
						}
						
						check=0;
					}
				
					list1 = list1->next;
				}

				list1=temp;
			}
           
            list2 = list2->next;
		}
		
		if(i>0){ //The days of sessions will be added to the list whenever there are days to add
			
		the_list = AFDATES_add(the_list, numruc, namaf, datedays, i);	
		}

		numruc=0;
		calend_control=0;
		count=1;
		lowestsession=1000;
		i=0;
		list2=temp2;
		
        for(w=0; w<31;w++){ //Updating the calendar value
        
        calend_control=calend_control+calend[w];
		}
   
		// The list of notes is released to be able to update the notes and the order
		GRADE_free(list2);
		list2=GRADE_read(list, list1, stdout);
		sortlist(list2);
	}
	
    return the_list;
}

// Function that goes popular calendar for the maximum of vacancies that will be available for training activities
void POPULATE_calendar(L_grade_data *list2, L_af_data *list1, int k, int *calend, FILE *fout)
{ 
	L_af_data *temp = list1;
    int i=0;
    int j;
    
    while (list2){
    
		while(list1){
                
			if(list2->ucnumber==list1->number_uc && list1->done==0){ //fills with slots whenever an activity appears in this space never ultcoming 2 slots
                    
				for(j=list1->start; j<=list1->end; j++){
                        
					if(calend[j]<2){
						calend[j]=calend[j]+1;
					}
				}
			}
              
			list1 = list1->next;
		}
		list1=temp;
		list2 = list2->next;
	}
}

// function that reorders the list of formative activities in increasing order
void sortlist_af_dates(L_af_dates_data *list){

    L_af_dates_data *new=list;
    
    while (new != NULL){
		
        L_af_dates_data * new1 = new->next;
            
        while (new1 != NULL){
				
			if (atoi(new->nam_af + 2)> atoi(new1->nam_af + 2)){ // if the activity is greater then change of position
                    
				int temp_ucnumber = new->ucnumb;
				new->ucnumb = new1->ucnumb;
				new1->ucnumb = temp_ucnumber;
				new->ucnumb = new1->ucnumb;
				change_string(new->nam_af, new1->nam_af, MAXSTR); 
				int *temp_days = NULL;
				temp_days = (int*)malloc(((new->i-1)+1)*sizeof(int));
				memcpy(temp_days, new->days, ((new->i-1)+1)*sizeof(int));
				new->days=(int *)realloc(new->days,((new1->i-1)+1)*sizeof(int));
				memcpy(new->days, new1->days, ((new1->i-1)+1)*sizeof(int));
				new1->days=(int *)realloc(new1->days,((new->i-1)+1)*sizeof(int));
				memcpy(new1->days, temp_days, ((new->i-1)+1)*sizeof(int));
				int temp_i = new->i; 
				new->i = new1->i;
				new1->i = temp_i;
					
				free(temp_days);
			}
			new1 = new1->next;
		}
			
        new = new->next;
	}
}

// Function that printed final results of sessions
void AFDATES_show(L_af_dates_data *list, L_grade_data *list2, int k, FILE *fout)
{
	L_af_dates_data *temp=list;
	int z;

	while(list2){ // Prints UC data with note
		
		fprintf(fout, "\n%s: UC%d %.1lf%% Nota prevista: %d valores", list2->ucname, list2->ucnumber, list2->percent, list2->grade);
		
		while (list) {
		
			if(list2->ucnumber==list->ucnumb){ // When the UC has scheduled sessions print the same

				fprintf(fout, "\n   %s, sessoes:", list->nam_af);
        
				for(int i=0; i<=list->i-1 ; i++){
						
					fprintf(fout, " %d", list->days[i]);
				}
			}
        
			list = list->next;
		}
		list = temp;
		list2 = list2->next;
	}
}

//function main
int main()
{
    int k=-1; // Define the K for negative paraser value used as a criterion
    int *calendar;
	int *checkline; //variable to check if there is empty line
    calendar = malloc (32 * sizeof (int)); // assign a calendario
	checkline = malloc (sizeof (int));
	checkline[0]=1;
	
	// Calls to read the list of input
    L_uc_data *list = UC_read(checkline,stdin); 
    L_af_data *list1 = AF_read(stdin);
    L_k_data *list3 = K_read(stdin);
	  
    while (list3) { // Save or K value
        k=list3->k;
        list3 = list3->next;
    }

    if(k<0){ // When the k does not exist

		UC_show(list, list1, checkline, k, stdout); // Prints the result
		AF_free(list1); // releases lists
		UC_free(list);
		K_free(list3);
    }
	
    if(k==0){ // When K is 0
	
		UC_show(list, list1, checkline, k, stdout); // Prints first result
		L_grade_data *list2=GRADE_read(list, list1, stdout); // Guard the list with calculated notes
		sortlist(list2); // organize the list
		GRADE_show(list2, stdout); // Show list
		GRADE_free(list2); //releases the lists
		AF_free(list1);
		UC_free(list);
		K_free(list3);
    }
	
    if(k>0){ //When K is greater than 0

		UC_show(list, list1, checkline, k, stdout); //Prints first result
        L_grade_data *list2=GRADE_read(list, list1, stdout); //Guard the list with calculated notes
        sortlist(list2); // organize the list
        POPULATE_calendar(list2, list1, k, calendar, stdout); //populate the calendar
        L_af_dates_data *list4=GET_af(list2, list1, list, k, calendar, stdout); // Generates list with scheduled sessions
		AF_free(list1); //Releases not necessary lists
		UC_free(list);
		K_free(list3);
		sortlist_af_dates(list4);
        AFDATES_show(list4, list2, k, stdout); //Show results
		free(calendar); //Releases lists and variables
		free(checkline);
		GRADE_free(list2);
		AFDATES_free(list4);
    }
 
    return 0;
}