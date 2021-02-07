///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Graph μέσω λιστών γειτνίασης.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <limits.h>

#include "ADTGraph.h"
#include "ADTPriorityQueue.h"


// Ένα Graph είναι pointer σε αυτό το struct
struct graph {
    Map map;                    // Κάθε κορυφή λειτουργεί σαν key που κάνει hash σε μία θέση του map, στην οποία και αποθηκεύεται το αντίστοιχο GraphNode
    CompareFunc compare;        // Συνάρτηση για σύγκριση δεικτών, που πρέπει να δίνεται απο τον χρήστη
    DestroyFunc destroy_vertex; // Συνάρτηση που καταστρέφει μία κορυφή της γράφου
    HashFunc hash_func;         // Συνάρτηση για να παίρνουμε το hash code του κάθε αντικειμένου
};

typedef struct graph_node* GraphNode;

struct  graph_node{
    Pointer value;      // Η κορυφή
    List neighbors;     // Η λίστα γειτνίασης
};

typedef struct edge* Edge;

struct edge {           // Δομή που αντιπροσωπεύει την ακμή δύο κορυφών -- εισάγεται στη λίστα γειτνίασης μίας κουρφής 
    Pointer point;      // Η κορυφή με την οποία συνδέεται 
    uint weight;        // Το βάρος της ακμής
};



Graph graph_create(CompareFunc compare, DestroyFunc destroy_vertex) {
    Graph graph = malloc(sizeof(*graph));
    graph->map = map_create(compare, destroy_vertex, free);     // destroy_value -> free διότι θέλουμε να απελευθερώνονται τα GraphNodes
    graph->compare = compare;
    graph->destroy_vertex = destroy_vertex;

    return graph;
}

int graph_size(Graph graph){
    return map_size(graph->map);
}

void graph_insert_vertex(Graph graph, Pointer vertex) {
    MapNode flag = map_find_node(graph->map, vertex);   // Έλεγχος για το αν υπάρχει ήδη στο map
    if(!flag) {     // Αν δεν υπάρχει δημιουργούμε ένα νέο GraphNode, αρχικοποιούμε τα στοιχεία του και το εισάγουμε στην θέση που κάνει hash η συγκεκριμένη κορυφή
        GraphNode node = malloc(sizeof(*node));
        node->value = vertex;
        node->neighbors = list_create(free);
        map_insert(graph->map, vertex, node);
    }
    else {         // Αν υπάρχει κάνουμε destroy την παλιά κορυφή και την αντικαθιστούμε με την νέα (ΔΕΝ πειράζουμε την λίστα γειτνίασης!)          
        GraphNode temp = map_node_value(graph->map, flag);
        if(graph->destroy_vertex != NULL) 
            graph->destroy_vertex(temp->value);
        temp->value = vertex;
    }

}

List graph_get_vertices(Graph graph) {
// Προσπελαύνουμε το map και εισάγουμε στη λίστα τις κορυφές
    List list = list_create(NULL);
    for(MapNode node = map_first(graph->map);
        node != MAP_EOF;
        node = map_next(graph->map, node)) { 
            GraphNode temp = map_node_value(graph->map, node);      
            list_insert_next(list, list_last(list), temp->value);
    }  
    return list;
}

void graph_remove_vertex(Graph graph, Pointer vertex) {
    MapNode find = map_find_node(graph->map, vertex);
    if(find) {      // Αν υπάρχει η κουρφή
        GraphNode to_remove = map_node_value(graph->map, find);
        for(MapNode node = map_first(graph->map);   // Προσπελαύνουμε το map και αφαιρούμε όλες τις ακμές μεταξύ της συγκεκριμένης κουρφής και των άλλων 
            node != MAP_EOF;
            node = map_next(graph->map, node)) {
                GraphNode temp = map_node_value(graph->map, node);
                if(graph->compare(temp->value, vertex) != 0)    // Όχι στην ίδια την κορυφή
                    graph_remove_edge(graph, vertex, temp->value);
                
        }
        list_destroy(to_remove->neighbors);     // Καταστρέφουμε τη λίστα γειτνίασης
        map_remove(graph->map, vertex);         // Την αφαιρούμε από το map (έχουμε map_destroy_value -> graph_destroy_value άρα απελευθερώνεται η κορυφή αναλόγως)
    }
}

void graph_insert_edge(Graph graph, Pointer vertex1, Pointer vertex2, uint weight) {
// Προσθέτουμε στις λίστες γειτνίασης και των δύο κορυφών, Edges που περιέχουν την κορυφή με την οποία συνδέονται και το βάρος της ακμής
// Αν υπάρχει ήδη αντίστοιχο Edge αντικαθιστούμε το παλιό βάρος με το νέο
    MapNode node1 = map_find_node(graph->map, vertex1);
    GraphNode temp1 = map_node_value(graph->map, node1);
    bool flag = true;
    for(ListNode node = list_first(temp1->neighbors);
        node != LIST_EOF;
        node = list_next(temp1->neighbors, node)) {
            Edge temp = list_node_value(temp1->neighbors, node);
            if(graph->compare(temp->point, vertex2) == 0) {
                flag = false;
                temp->weight = weight;
                break;
            }
    }
    if(flag) {  // Δεν υπάρχει παλιό Edge
        Edge line1 = malloc(sizeof(*line1));
        line1->point = vertex2;
        line1->weight = weight;
        list_insert_next(temp1->neighbors, list_last(temp1->neighbors), line1);
    }

    MapNode node2 = map_find_node(graph->map, vertex2);
    GraphNode temp2 = map_node_value(graph->map, node2);
    flag = true;
    for(ListNode node = list_first(temp2->neighbors);
        node != LIST_EOF;
        node = list_next(temp2->neighbors, node)) {
            Edge temp = list_node_value(temp2->neighbors, node);
            if(graph->compare(temp->point, vertex1) == 0) {
                flag = false;
                temp->weight = weight;
                break;
            }
    }
    if(flag) {
        Edge line2 = malloc(sizeof(*line2));
        line2->point = vertex1;
        line2->weight = weight;
        list_insert_next(temp2->neighbors, list_last(temp2->neighbors), line2);
    }
}


void graph_remove_edge(Graph graph, Pointer vertex1, Pointer vertex2) {
// Αφαιρούμε από τις λίστες γειτνίασης και των δύο κορυφών, τα Edges που περιέχουν την κορυφή με την οποία συνδέονται και το βάρος της ακμής 
    MapNode node1 = map_find_node(graph->map, vertex1);
    GraphNode temp1 = map_node_value(graph->map, node1);
    ListNode prev1 = NULL;
    for(ListNode node = list_first(temp1->neighbors);
        node != LIST_EOF;
        node = list_next(temp1->neighbors, node)) {
            Edge temp = list_node_value(temp1->neighbors, node);
            if(graph->compare(temp->point, vertex2) == 0) {
                list_remove_next(temp1->neighbors, prev1);
                break;
            }						
            prev1 = node;	// Ο προηγούμενος του node
    }

    MapNode node2 = map_find_node(graph->map, vertex2);
    GraphNode temp2 = map_node_value(graph->map, node2);
    ListNode prev2 = NULL;
    for(ListNode node = list_first(temp2->neighbors);
        node != LIST_EOF;
        node = list_next(temp2->neighbors, node)) {
            Edge temp = list_node_value(temp2->neighbors, node);
            if(graph->compare(temp->point, vertex1) == 0) {
                list_remove_next(temp2->neighbors, prev2);
                return;
            }						
            prev2 = node;	// Ο προηγούμενος του node
    }
}

uint graph_get_weight(Graph graph, Pointer vertex1, Pointer vertex2) {
// Προσπελαύνουμε την λίστα γειτνίασης ενός εκ των δύο κορυφών και επιστρέφουμε το βάρος της ακμής τους αν είναι συνδεδεμένες αλλιώς UINT_MAX
    if(vertex1 == vertex2)
        return 0;       // Το βάρος από μία κορυφή στον εαυτό της είναι 0

    MapNode node = map_find_node(graph->map, vertex1);  // Θεωρώ πως οι κορυφές ανήκουν στη γράφο διαφορετικά έχω απροσδιόριστη συμπεριφορά
    GraphNode temp = map_node_value(graph->map, node);
    for(ListNode node = list_first(temp->neighbors);
        node != LIST_EOF;
        node = list_next(temp->neighbors, node)) {
            Edge line = list_node_value(temp->neighbors, node);
            if(graph->compare(line->point, vertex2) == 0) 
                return line->weight;

    } 
    return UINT_MAX;
}

List graph_get_adjacent(Graph graph, Pointer vertex) {
    List list = list_create(NULL);
    MapNode node = map_find_node(graph->map, vertex);
    if(node) {  // Αν υπάρχει η κορυφή στη γράφο εισάγουμε στη λίστα τις κορυφές που περιέχει η λίστα γειτνίασης
        GraphNode temp = map_node_value(graph->map, node);
        for(ListNode node = list_first(temp->neighbors);
            node != LIST_EOF;
            node = list_next(temp->neighbors, node)) {
                Edge line = list_node_value(temp->neighbors, node);
                list_insert_next(list, list_last(list), line->point);
        }
    }
    return list;
}

typedef struct info* Info;
// Βοηθητική δομή που αποτελεί το value της θέσης του map στην οποία κάνει hash το key = vertex
struct info {
    bool visited;       // Έλεγχος αν έχει επισκεφτεί το εκάστοτε vertex
    int distance;       // Συντομότερο μονοπάτι από το source (ξέρω ότι το βάρος είναι ακέραιος αριθμός άρα τον ίδιο τύπο θα έχει και η απόσταση)
    Pointer previous;   // Προηγούμενο vertex του εκάστοτε μονοπατιού
    Pointer vertex;     // Δείκτης στην κορυφή -- χρησιμεύει στην ουρά προτεραιότητας  
    PriorityQueueNode keep; // Δείκτης στον κόμβο της ουράς -- χρησιμεύει στην pq_update_order
};

// Συνάρτηση compare ώστε η ουρά προτεραιότητας να διαμορφώνεται με βάση το distance 
static int reverse_compare(Info a, Info b) { 
    int x,y;        
    x = a->distance;
    y = b->distance;
    return y - x;       // Προκειμένου να έχουμε φθίνουσα σειρά
}

List graph_shortest_path(Graph graph, Pointer source, Pointer target) {
    List list =  list_create(NULL);
    Map map_info = map_create(graph->compare, NULL, free);
    PriorityQueue pq = pqueue_create((CompareFunc) reverse_compare, NULL, NULL);    

    map_set_hash_function(map_info, graph->hash_func);

    // Αρχικοποίηση
    Info temp = malloc(sizeof(*temp));
    temp->distance = 0;
    temp->previous = NULL;
    temp->visited = false;
    temp->vertex = source;
    temp->keep = pqueue_insert(pq, temp); 

    // Τόσο στην ουρά όσο και στο νέο map περνάμε δείκτες τύπου Info
    map_insert(map_info, source, temp);         
    
    Info current;   

    while(pqueue_size(pq) > 0) {    // Όσο η ουρά περιέχει στοιχεία
        current = pqueue_max(pq);
        pqueue_remove_max(pq);  

        if(current->visited == true)    // Ελέγχουμε αν το έχουμε επισκεφτεί στο παρελθόν 
            continue;
        current->visited = true;        

        if(current->vertex == target)   // Αν βρήκαμε την κουρφή-προορισμό σταματάμε (έχουμε κρατήσει τον αντίστοιχο δείκτη Info στο current)
            break;

        List neigh = graph_get_adjacent(graph, current->vertex);    // Η λίστα γειτνίασης της εκάστοτε κουρφής
        if(list_size(neigh) > 0) {      // Αν έχει τουλάχιστον ένα γείτονα
            for(ListNode node = list_first(neigh);  node != LIST_EOF;  node = list_next(neigh, node)) {
                Pointer nd_temp = list_node_value(neigh, node);
                MapNode check = map_find_node(map_info, nd_temp);
                Info flag;  
                if(check == NULL) {     // Αν δεν υπάρχει στο map με τα Info το κάνουμε insert τόσο σε αυτό όσο και στην ουρά
                    // Αρχικοποίηση -- αποφεύγεται και ο έλεγχος !exist(dist[v]) με αυτό τον τρόπο
                    Info temp = malloc(sizeof(*temp));
                    temp->distance = graph_get_weight(graph, current->vertex, nd_temp);
                    temp->previous = current->vertex;
                    temp->visited = false;
                    temp->vertex = nd_temp;
                    temp->keep = pqueue_insert(pq, temp);
                    
                    flag = temp;

                    map_insert(map_info, nd_temp, temp);
                    
                }
                else {          // Διαφορετικά ελέγχουμε αν το έχουμε επισκεφτεί στο παρελθόν 
                    flag = map_node_value(map_info, check);
                    if(flag->visited == true)
                        continue;
                }

                int weight = graph_get_weight(graph, current->vertex, nd_temp); // βάρος ακμής της τρέχουσας κορυφής με τον εκάστοτε γείτονα
                int alt = current->distance + weight;  // εναλλακτικό μονοπάτι      

                // Αν το εναλλακτικό μονοπάτι κοστίζει λιγότερο από το υπάρχον ενημερώνουμε τα distance, previous καθώς και την ουρά
                if(alt < flag->distance) {  
                    flag->distance = alt;
                    flag->previous = current->vertex;
                    pqueue_update_order(pq, flag->keep);

                }
            }
        }
        
        list_destroy(neigh);

    }

    // Προσθέτουμε τον προηγούμενο του current( = target) - βάσει των πληροφοριών στο map με τα Info - στην αρχή της λίστας
    // Επαναλαμβάνουμε την ίδια διαδικασία μέχρις ότου να φτάσουμε στην κορυφή-αφετηρία
    Pointer return_to_src = current->previous;      
    list_insert_next(list, LIST_BOF, return_to_src);

    while(return_to_src != source) {
        MapNode find = map_find_node(map_info, return_to_src);
        Info temp = map_node_value(map_info, find);
        return_to_src = temp->previous;
        if(return_to_src != source)
            list_insert_next(list, LIST_BOF, return_to_src);
    }

    pqueue_destroy(pq);     // Απελευθέρωση μνήμης 
    map_destroy(map_info);

    return list;
}

void graph_destroy(Graph graph) {
// Προσπελαύνουμε όλο το map και απελευθερώνουμε τις λίστες γειτνίασης κάθε κορυφής
    for(MapNode node = map_first(graph->map);
        node != MAP_EOF;
        node = map_next(graph->map, node)) { 
            GraphNode temp = map_node_value(graph->map, node);
            list_destroy(temp->neighbors);
    }   
    map_destroy(graph->map);    // Καταστρέφουμε το map και μαζί του όλες τις κορυφές
    free(graph);                
}

void graph_set_hash_function(Graph graph, HashFunc hash_func) {
    graph->hash_func = hash_func;   
    map_set_hash_function(graph->map, hash_func);
}
