# import packages
library(sf)
library(sp)

# load data
parcel_shape <- read_sf(dsn = "~/Desktop/Spring2023/advacne_econometrix/summer_research/2nd part/data/parcels_Denver", layer = "parcels_Denver")
sub_shape <- read_sf(dsn = "~/Desktop/Spring2023/advacne_econometrix/summer_research/2nd part/data/parcels_Denver", layer = "subdivisions")
parcels_class <- readRDS("~/Desktop/Spring2023/advacne_econometrix/summer_research/2nd part/data/parcels_Denver/parcels_class.rds")

# subset to work with smaller samples
a <- parcel_shape[0:100,]
b <- parcels_class[0:100,]
c <- merge(a, b, by.x = "prcls_r", by.y = "prcls_r")

# set all spacial data with same CRS standard
a  <- a %>% st_transform(4326)
c  <- c %>% st_transform(4326)
sub_shape <- sub_shape %>% st_transform(4326)

# visualize
plot(sub_shape$geometry)
plot(c)

# turn off spherical geometry to avoid edge crossing
sf_use_s2()
st_is_longlat(sub_shape)
sf_use_s2(FALSE)

# find centroids of the different parcel
centroids  <- sf::st_centroid(c)

# find out to which subdivision they belong
inters <- sf::st_intersection(sub_shape, centroids) %>% 
  sf::st_set_geometry(NULL)

# re-join with the parcel
out <- dplyr::left_join(c, inters) %>% 
  sf::st_sf(sf_column_name = "geometry")
head(out)

# mean and quantiles, variance, of class by subdivision
for (x in neighborhoods) {
  print(x)
  print(mean(subset(out, SUBNAME == x)$class, na.rm = T))
  print(quantile(subset(out, SUBNAME == x)$class, probs = seq(0, 1, 0.25), na.rm = FALSE))
  print(var(subset(out, SUBNAME == x)$class, na.rm = T))
} 

# for each parcel, get num of obs that's class=1 and within a 1km radius
circle_out <- out %>% st_buffer(1)
out[["close_id"]] <- st_within(out, circle_out) %>% imap(setdiff)
plot(circle_out)
out[["numOfObs"]] <- out[["close_id"]] %>% map_int(length)
num <- 0
i <- 1
res <- list()
for (list in out[["close_id"]]) {
  for (row in list) {
    if (out[row,'class']$class == 1){
      num <- num+1
    } 
  }
  res[i] <- num
  i <- i+1
  num <- 0
} 
out[['numOfclass1']] <- res






































