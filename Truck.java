public class Truck {
    private int cost;
    private Client[] clients;
    
    public Truck() {}

    public int getCost() {
        return cost;
    }

    public void setCost(int newCost) {
        this.cost = newCost;
    }

    public Client[] getClients() {
        return clients;
    }

    public void setClients(Client[] clients) {
        this.clients = clients;
    }
}