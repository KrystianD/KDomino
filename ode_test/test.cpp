#include <ode/ode.h>
#include <unistd.h>
// dynamics and collision objects
static dWorldID world;
static dSpaceID space;
static dBodyID body;
static dGeomID geom;
static dMass m;
static dJointGroupID contactgroup;

// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
static void nearCallback (void *data, dGeomID o1, dGeomID o2){
	printf("nearCallback\n");
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	dContact contact;
	contact.surface.mode = dContactBounce | dContactSoftCFM;
	// friction parameter
	contact.surface.mu = dInfinity;
	// bounce is the amount of "bouncyness".
	contact.surface.bounce = 0.9;
	// bounce_vel is the minimum incoming velocity to cause a bounce
	contact.surface.bounce_vel = 0.1;
	// constraint force mixing parameter
	contact.surface.soft_cfm = 0.001;  
	if(int numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact))) {
		dJointID c = dJointCreateContact (world,contactgroup,&contact);
		dJointAttach (c,b1,b2);
	}
}

// simulation loop
static void simLoop (int pause){
	const dReal *pos;
	const dReal *R;
	// find collisions and add contact joints
	dSpaceCollide (space,0,&nearCallback);
	// step the simulation
	dWorldQuickStep (world,0.01);  
	// remove all contact joints
	dJointGroupEmpty (contactgroup);
	// redraw sphere at new location
	pos = dGeomGetPosition (geom);
	R = dGeomGetRotation (geom);
	
	printf("%f %f %f\n",pos[0],pos[1],pos[2]);
	
	//double worldPos = dGeomGetPosition(world);
	//dsDrawSphere (pos,R,dGeomSphereGetRadius (geom));
}

// start simulation - set viewpoint
static void start(){
	//static float xyz[3] = {2.0f,-2.0f,1.7600f};
	//static float hpr[3] = {140.000f,-17.0000f,0.0000f};
	//dsSetViewpoint (xyz,hpr);
}

int main (int argc, char **argv){
	dInitODE ();
	// create world
	world = dWorldCreate ();
	space = dHashSpaceCreate (0);
	dWorldSetGravity (world,0,0,-0.2);
	dWorldSetCFM (world,1e-5);
	dCreatePlane (space,0,0,1,0);
	contactgroup = dJointGroupCreate (0);
	// create object
	body = dBodyCreate (world);
	geom = dCreateSphere (space,0.5);
	dMassSetSphere (&m,1,0.5);
	dBodySetMass (body,&m);
	dGeomSetBody (geom,body);
	// set initial position
	dBodySetPosition (body,0,0,3);
	// run simulation
	start();
	for(;;){
		simLoop(0);
		usleep(10000);
	}
	// clean up
	dJointGroupDestroy (contactgroup);
	dSpaceDestroy (space);
	dWorldDestroy (world);
	dCloseODE();
	return 0;
}